// SYSTEM INCLUDES
#include <algorithm>                    // std::find_if
#include <stdexcept>                    // std::logic_error
#include <iostream>

// C++ PROJECT INCLUDES
#include "Logging/Factory.hpp"          // Logging::Factory::MakeLogger
#include "Logging/ILogger.hpp"          // Logging macros

#include "RobosConfig.hpp"
#include "Robos/MasterNode.hpp"

namespace Robos
{
namespace Internal
{

    MasterNode::MasterNode() : _pUnprotectedNodeDB(std::make_shared<NodeDatabase>()),
        _readCount(0), _resourceAccess(1),
        _readCountAccess(1), _serviceQueue(1), _initFlag(false), _initNodes()
    {
    }

    MasterNode::~MasterNode()
    {
    }

    unsigned int MasterNode::HashTopic(const std::string& topic)
    {
        unsigned int hash = 0;
        int count = 0;
        for(auto rit = topic.rbegin(); rit != topic.rend(); ++rit)
        {
            hash += (*rit) * (31 ^ (count));
        }
        return hash;
    }

    void MasterNode::Stop()
    {
        this->_initFlag = false;

        this->_serviceQueue.wait();
        this->_readCountAccess.wait();
        if(this->_readCount == 0)
        {
            this->_resourceAccess.wait();
        }
        this->_readCount++;
        this->_serviceQueue.signal();
        this->_readCountAccess.signal();

        this->_pUnprotectedNodeDB->map.clear();

        this->_readCountAccess.wait();
        this->_readCount--;
        if(this->_readCount == 0)
        {
            this->_resourceAccess.signal();
        }
        this->_readCountAccess.signal();
    }

    void MasterNode::Start()
    {
        if(!this->_initFlag)
        {
            this->_initFlag = true;
            for(InitNodeBasePtr& pInitNodeBase : this->_initNodes)
            {
                Async::Execute<int>([pInitNodeBase]() -> int
                {
                    return pInitNodeBase->MainCallback();
                }, pInitNodeBase->GetExecutionTopic(), pInitNodeBase->GetPriority());
            }
        }
    }

    bool MasterNode::Register(const NodeBasePtr pNode)
    {
        if(!this->_initFlag)
        {
            // std::string loggingPath = Robos::Config::LOGGING_ROOT + "/MasterNodeLog.txt";
            // Logging::LoggerPtr pLogger = Logging::Factory::MakeLogger("MasterNode::Register()",
            //                                                           loggingPath.c_str());
            // LOG_DEBUG(pLogger, "Entering %s", "MasterNode::Register()");

            bool modification = true;
            this->_serviceQueue.wait();
            this->_resourceAccess.wait();
            this->_serviceQueue.signal();

            // LOG_DEBUG(pLogger, "%s", "entering critical section");
            //--------------CRITICAL SECTION----------------
            ///*
            const std::string& nodeName = pNode->GetName();
            // LOG_DEBUG(pLogger, "name of node: %s", nodeName.c_str());
            //unsigned int hash;
            for(const std::string& subscription : pNode->GetSubscriptions())
            {
                //hash = this->HashTopic(subscription);
                // LOG_DEBUG(pLogger, "subscription: %s", subscription.c_str());
                //NodeDatabaseImpl::iterator it = this->_pUnprotectedNodeDB->map.find(hash);
                NodeDatabaseImpl::iterator it = this->_pUnprotectedNodeDB->map.find(subscription);
                if(!this->_pUnprotectedNodeDB->map.empty() &&
                   it != this->_pUnprotectedNodeDB->map.end())
                {
                    // LOG_DEBUG(pLogger, "subscription: [%s] already exists! Checking if node already exists", subscription.c_str());
                    std::vector<NodeBasePtr>* subscribers = &it.data();

                    // check if pNode already is already existing as a subscriber here.
                    if(std::find_if(subscribers->begin(), subscribers->end(),
                       [nodeName](NodeBasePtr& pNode) -> bool
                    {
                        return pNode->GetName() == nodeName;
                    }) != subscribers->end())
                    {
                        modification = false;
                    }
                    else
                    {
                        subscribers->push_back(pNode);
                    }
                }
                else
                {
                    // LOG_DEBUG(pLogger, "adding subscription: [%s] to database.", subscription.c_str());
                    // add the <subscription, std::vector<NodeBasePtr>{pNode}>
                    std::vector<NodeBasePtr> subscribers;
                    subscribers.push_back(pNode);
                    /*
                    if(!this->_pUnprotectedNodeDB->map.insert(hash, subscribers).second)
                    {
                    modification = false;
                    }
                    */
                    this->_pUnprotectedNodeDB->map.insert(
                        //std::pair<unsigned int, std::vector<NodeBasePtr> >(hash, subscribers));
                        std::pair<std::string, std::vector<NodeBasePtr> >(subscription, subscribers));
                }
            }

            this->_resourceAccess.signal();
            return modification;
        }
        return false;
    }

    bool MasterNode::RegisterInitNode(const InitNodeBasePtr pNode)
    {
        if(!this->_initFlag)
        {
            this->_initNodes.push_back(pNode);
            return true;
        }
        return false;
    }

    void MasterNode::InvokeSubscribers(const MessageBasePtr pMessage)
    {
        if(pMessage)
        {
            this->_serviceQueue.wait();
            this->_readCountAccess.wait();
            if(this->_readCount == 0)
            {
                this->_resourceAccess.wait();
            }
            this->_readCount++;
            this->_serviceQueue.signal();
            this->_readCountAccess.signal();

            //--------------CRITICAL SECTION----------------
            //std::cout << "pMessage use count: " << pMessage.use_count() << std::endl;
            // std::cout << "MasterNode::InvokeSubscribers() Entering Critical Section" << std::endl;
            //auto iterator = this->_pUnprotectedNodeDB->map.find(this->HashTopic(pMessage->topic));
            auto iterator = this->_pUnprotectedNodeDB->map.find(pMessage->topic);
            if(iterator != this->_pUnprotectedNodeDB->map.end())
            {
                for(NodeBasePtr pNodeBase : iterator->second)
                {
                    Async::Execute<MessageBasePtr>([pMessage, pNodeBase]() -> MessageBasePtr
                    {
                        return pNodeBase->MainCallback(pMessage);
                    }, pNodeBase->GetExecutionTopic(), pNodeBase->GetPriority())->Then<bool>([this](MessageBasePtr pMessage) -> bool
                    {
                        this->InvokeSubscribers(pMessage);
                        return true;
                    }, pNodeBase->GetExecutionTopic());
                }
            }
            //----------------------------------------------

            this->_readCountAccess.wait();
            this->_readCount--;
            if(this->_readCount == 0)
            {
                this->_resourceAccess.signal();
            }
            this->_readCountAccess.signal();
        }
    }

} // end of namespace Internal
} // end of namespace Robos

// SYSTEM INCLUDES
#include <algorithm>                    // std::find_if
#include <stdexcept>                    // std::logic_error

// SUPER JENKY HACK!!!!!!!!!!!!!!!!!!!!!!
#include <chrono>
#include <thread>
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// C++ PROJECT INCLUDES
#include "Logging/Factory.hpp"          // Logging::Factory::MakeLogger
#include "Logging/ILogger.hpp"          // Logging macros

#include "RobosLoggingConfig.hpp"
#include "Robos/MasterNode.hpp"

namespace Robos
{
namespace Internal
{

    MasterNode::MasterNode() : _pUnprotectedNodeDB(std::make_shared<NodeDatabase>()), _readCount(0),
        _resourceAccess(1), _readCountAccess(1), _serviceQueue(1)
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
            hash += (*rit) * (31 ^ count);
        }
        return hash;
    }

    bool MasterNode::Register(const NodeBasePtr pNode)
    {
        std::string loggingPath = Robos::LoggingConfig::LOGGING_ROOT + "/MasterNodeLog.txt";
        Logging::LoggerPtr pLogger = Logging::Factory::MakeLogger("MasterNode::Register()",
            loggingPath.c_str());
        LOG_DEBUG(pLogger, "Entering %s", "MasterNode::Register()");

        bool modification = true;
        this->_serviceQueue.wait();
        this->_resourceAccess.wait();
        this->_serviceQueue.signal();

        LOG_DEBUG(pLogger, "%s", "entering critical section");
        //--------------CRITICAL SECTION----------------
        ///*
        const std::string& nodeName = pNode->GetName();
        LOG_DEBUG(pLogger, "name of node: %s", nodeName.c_str());
        unsigned int hash;
        for(const std::string& subscription : pNode->GetSubscriptions())
        {
            hash = this->HashTopic(subscription);
            LOG_DEBUG(pLogger, "subscription: %s", subscription.c_str());
            NodeDatabaseImpl::iterator it = this->_pUnprotectedNodeDB->map.find(hash);
            if(!this->_pUnprotectedNodeDB->map.empty() &&
               it != this->_pUnprotectedNodeDB->map.end())
            {
                LOG_DEBUG(pLogger, "subscription: [%s] already exists! Checking if node already exists", subscription.c_str());
                std::vector<NodeBasePtr> subscribers = it->second;//.data();

                // check if pNode already is already existing as a subscriber here.
                if(std::find_if(subscribers.begin(), subscribers.end(),
                    [nodeName](NodeBasePtr& pNode) -> bool
                {
                    return pNode->GetName() == nodeName;
                }) != subscribers.end())
                {
                    modification = false;
                }
            }
            else
            {
                LOG_DEBUG(pLogger, "adding subscription: [%s] to database.", subscription.c_str());
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
                    std::pair<unsigned int, std::vector<NodeBasePtr> >(hash, subscribers));
            }
        }
        //----------------------------------------------
        //*/

        this->_resourceAccess.signal();
        return modification;
    }

    void MasterNode::InvokeSubscribers(const MessageBasePtr pMessage)
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
        auto iterator = this->_pUnprotectedNodeDB->map.find(this->HashTopic(pMessage->topic));
        if(iterator != this->_pUnprotectedNodeDB->map.end())
        {
            for(NodeBasePtr pNodeBase : iterator->second)
            {
                Async::Execute<MessageBasePtr>([pMessage, pNodeBase]() -> MessageBasePtr
                {
                    return pNodeBase->MainCallback(pMessage);
                }, pNodeBase->GetExecutionTopic());
                /*
                ->Then<bool>([this](MessageBasePtr pMessage) -> bool
                {
                    this->InvokeSubscribers(pMessage);
                    return true;
                }, pNodeBase->GetExecutionTopic());
                */

                // THIS LINE IS A TEMPORARY FIX...LIKE ON THE SAME SCALE OF FIXING
                // A CAR WITH DUCT TAPE.
                std::this_thread::sleep_for(std::chrono::seconds(1));
                // This is having an issue on linux (a segmentation fault)
                // because SAVING the Promise!!! The scenario exists
                // where the Async is taking longer than the rest of this method's execution to fulfill
                // the Promise and the Promise (and lambda) have destructed before it can be fulfilled!!!!

                // I'm still not sure how to fix this. We could save the Promise in a database but that may
                // be memory intensive or we could come up with another algorithm for executing Client Nodes.
                // Or we could have Async track them....I also wonder if move semantics would be enough here
                // to preserve the lambda?
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

    bool MasterNode::Shutdown()
    {
        return false;
    }

} // end of namespace Internal
} // end of namespace Robos

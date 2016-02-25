// SYSTEM INCLUDES
#include <algorithm>                    // std::find_if
#include <stdexcept>                    // std::logic_error

// C++ PROJECT INCLUDES
#include "Robos/MasterNode.hpp"


namespace Robos
{
namespace Internal
{

    MasterNode::MasterNode() : _pUnprotectedNodeDB(), _readCount(0),
        _resourceAccess(1), _readCountAccess(1), _serviceQueue(1)
    {
    }

    MasterNode::~MasterNode()
    {
    }

    bool MasterNode::AddNode(const NodeBasePtr& pNode)
    {
        bool modification = true;
        this->_serviceQueue.wait();
        this->_resourceAccess.wait();
        this->_serviceQueue.signal();

        //--------------CRITICAL SECTION----------------
        ///*
        const std::string& nodeName = pNode->GetName();
        for(const std::string& subscription : pNode->GetSubscriptions())
        {
            auto iterator = this->_pUnprotectedNodeDB->map.find(subscription);
            if(iterator != this->_pUnprotectedNodeDB->map.end())
            {
                std::vector<NodeBasePtr>& subscribers = iterator.data();

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
                // add the <subscription, std::vector<NodeBasePtr>{pNode}>
                std::vector<NodeBasePtr> subscribers;
                subscribers.push_back(pNode);
                if(!this->_pUnprotectedNodeDB->map.insert(subscription, std::move(subscribers)).second)
                {
                   modification = false;
                }
            }
        }
        //----------------------------------------------
        //*/

        this->_resourceAccess.signal();
        return modification;
    }

    void MasterNode::InvokeSubscribers(const MessageBasePtr& pMessage)
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
        // get list of NodeBases that subscribe to message topic
        // and invoke them...give invoke this method in posterior function
        ///*
        auto iterator = this->_pUnprotectedNodeDB->map.find(pMessage->topic);
        if(iterator != this->_pUnprotectedNodeDB->map.end())
        {
            for(NodeBasePtr& pNodeBase : iterator.data())
            {
                Async::Execute<MessageBasePtr>([pMessage, &pNodeBase]() -> MessageBasePtr
                {
                    return pNodeBase->MainCallback(pMessage);
                }, pNodeBase->GetExecutionTopic())->Then<bool>([this](MessageBasePtr pMessage) -> bool
                {
                    this->InvokeSubscribers(pMessage);
                    return true;
                }, pNodeBase->GetExecutionTopic());
            }
        }
        //----------------------------------------------
        //*/

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

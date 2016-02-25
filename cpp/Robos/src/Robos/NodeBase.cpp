// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/NodeBase.hpp"

namespace Robos
{

    NodeBase::NodeBase(std::string name, std::string toRunOn,
                       std::vector<std::string> subscriptions) : _name(name),
        _executionTopic(toRunOn), _subscriptions(subscriptions)
    {
    }

    NodeBase::~NodeBase()
    {
    }

    const std::string& NodeBase::GetName()
    {
        return this->_name;
    }

    std::string& NodeBase::GetExecutionTopic()
    {
        return this->_executionTopic;
    }

    const std::vector<std::string>& NodeBase::GetSubscriptions()
    {
        return this->_subscriptions;
    }

    MessageBasePtr NodeBase::MainCallback(const MessageBasePtr& pMessage)
    {
        return this->MainCallbackImpl(std::forward<const MessageBasePtr&>(pMessage));
    }

}

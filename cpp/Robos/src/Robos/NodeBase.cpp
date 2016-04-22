// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/NodeBase.hpp"

namespace Robos
{

    NodeBase::NodeBase(std::string name, std::string toRunOn,
                       std::vector<std::string> subscriptions,
                       Async::Types::JobPriority priority) :
        NodeHelper(name, toRunOn, priority), _subscriptions(subscriptions)
    {
    }

    NodeBase::~NodeBase()
    {
    }

    const std::vector<std::string>& NodeBase::GetSubscriptions()
    {
        return this->_subscriptions;
    }

    MessageBasePtr NodeBase::MainCallback(const MessageBasePtr pMessage)
    {
        return this->MainCallbackImpl(std::forward<const MessageBasePtr>(pMessage));
    }

}

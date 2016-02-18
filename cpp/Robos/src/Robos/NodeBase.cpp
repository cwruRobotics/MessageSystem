// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/NodeBase.hpp"

namespace Robos
{

    NodeBase::NodeBase(std::string name) : _name(name)
    {
    }

    NodeBase::~NodeBase()
    {
    }

    MessageBasePtr NodeBase::MainCallback(MessageBasePtr& pMessage)
    {
        return this->MainCallbackImpl(pMessage);
    }

}

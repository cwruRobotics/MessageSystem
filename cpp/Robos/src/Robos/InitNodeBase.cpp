// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/InitNodeBase.hpp"
#include "Robos/MasterNode.hpp"

namespace Robos
{

    InitNodeBase::InitNodeBase(std::string name,
                               std::string toRunOn) :
        NodeHelper(name, toRunOn, Async::Types::JobPriority::OTHER)
    {
    }

    InitNodeBase::~InitNodeBase()
    {
    }

    int InitNodeBase::MainCallback()
    {
        this->MainCallbackImpl();
        return 0;
    }

    void InitNodeBase::PublishMessage(MessageBasePtr pMessage)
    {
        Publish(pMessage);
    }

}

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/InitNodeBase.hpp"
#include "Robos/MasterNode.hpp"

namespace Robos
{

    InitNodeBase::InitNodeBase(std::string name, std::string toRunOn) : _name(name),
        _executionTopic(toRunOn)
    {
    }

    InitNodeBase::~InitNodeBase()
    {
    }

    const std::string& InitNodeBase::GetName()
    {
        return this->_name;
    }

    std::string& InitNodeBase::GetExecutionTopic()
    {
        return this->_executionTopic;
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

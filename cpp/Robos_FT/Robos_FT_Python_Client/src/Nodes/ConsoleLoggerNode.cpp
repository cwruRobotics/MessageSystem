// SYSTEM INCLUDES
#include <iostream>

// C++ PROJECT INCLUDES
#include "Nodes/ConsoleLoggerNode.hpp"
#include "Messages/FactorialMessage.hpp"

namespace Robos_FT_Cpp_Client
{

    ConsoleLoggerNode::ConsoleLoggerNode() : Robos::NodeBase("ConsoleLoggerNode",
                                                             "ConsoleLoggerScheduler",
                                                             std::vector<std::string>{"FactorialMessage"})
    {
    }

    ConsoleLoggerNode::~ConsoleLoggerNode()
    {
    }

    Robos::MessageBasePtr ConsoleLoggerNode::MainCallbackImpl(const Robos::MessageBasePtr pMessage)
    {
        if(pMessage->topic == "FactorialMessage")
        {
            auto pCastedMessage = std::dynamic_pointer_cast<FactorialMessage>(pMessage);
            std::cout << pCastedMessage->factorialArg << "! = " << pCastedMessage->factorialResult << std::endl;
            return nullptr;
        }
        throw std::logic_error("Unknown message topic " + pMessage->topic);
    }
} // end of namespace Robos_FT_Cpp_Client

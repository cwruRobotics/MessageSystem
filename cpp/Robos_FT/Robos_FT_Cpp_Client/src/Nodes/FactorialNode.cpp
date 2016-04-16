// SYSTEM INCLUDES
//#include <stdexcept>
//#include <iostream>

// C++ PROJECT INCLUDES
#include "Nodes/FactorialNode.hpp"
#include "Messages/FactorialMessage.hpp"
#include "Messages/SensorMessage.hpp"

namespace Robos_FT_Cpp_Client
{

    FactorialNode::FactorialNode() : Robos::NodeBase("FactorialNode", "FactorialScheduler",
        std::vector<std::string>{"SensorMessage"})
    {
    }

    FactorialNode::~FactorialNode()
    {
    }

    int FactorialNode::ComputeFactorial(int n)
    {
        int factorial = 1;
        for(int i = 1; i <= n; ++i)
        {
            factorial *= i;
        }
        return factorial;
    }

    Robos::MessageBasePtr FactorialNode::MainCallbackImpl(const Robos::MessageBasePtr pMessage)
    {
        if(pMessage->topic == "SensorMessage")
        {
            int n = std::dynamic_pointer_cast<SensorMessage>(pMessage)->numToFactorial;
            // std::cout << "Computing factorial of" << n << std::endl;
            FactorialMessagePtr pFactorialMsg =
                std::make_shared<FactorialMessage>(n, this->ComputeFactorial(n));
            return pFactorialMsg;
        }
        throw std::logic_error("Unknown message topic " + pMessage->topic);
    }

} // end of namespace Robos_FT_Cpp_Client
// SYSTEM INCLUDES
#include <iostream>
#include <Robos/Robos.hpp>

// C++ PROJECT INCLUDES
#include "Nodes/SensorNode.hpp"
#include "Messages/SensorMessage.hpp"

namespace Robos_FT_Cpp_Client
{

    SensorNode::SensorNode() : Robos::InitNodeBase("SensorNode", "SensorScheduler")
    {   
    }

    SensorNode::~SensorNode()
    {
    }

    void SensorNode::MainCallbackImpl()
    {
        //std::cout << "Running SensorNode" << std::endl;
        int n = 0;
        while(Robos::IsRunning() && n >= 0)
        {
            std::cout << "Please enter a number to compute the factorial of (negative to quit)" << std::endl;
            std::cin >> n;
            if(n >= 0)
            {
                // std::cout << "Sending message with argument " << n << std::endl;
                auto sensorMessage = std::make_shared<SensorMessage>(n);
                this->PublishMessage(sensorMessage);
            }
        }
        this->ShutdownRobos();
    }

} // end of namespace Robos_FT_Cpp_Client
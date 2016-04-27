// SYSTEM INCLUDES
#include <chrono>
#include <iostream>
#include <thread>

// C++ PROJECT INCLUDES
#include "Robos/unitTest/ShutdownTestNode.hpp"

namespace Robos
{
namespace Tests
{

    ShutdownTestNode::ShutdownTestNode() : InitNodeBase("ShutdownTestNode", "TestScheduler")
    {
    }

    ShutdownTestNode::~ShutdownTestNode()
    {
    }

    void ShutdownTestNode::MainCallbackImpl()
    {
        std::cout << "ShutdownTestNode waiting for 2 seconds to tell Robos to shut down" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Telling Robos to shut down" << std::endl;
        this->ShutdownRobos();
    }

} // end of namespace Tests
} // end of namespace Robos

// SYSTEM INCLUDES
#include <Robos/Robos.hpp>
#include <Utilities/OSUtils.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>
#include <chrono>

// C++ PROJECT INCLUDES
#include "Nodes/SensorNode.hpp"
#include "Nodes/FactorialNode.hpp"
#include "Nodes/ConsoleLoggerNode.hpp"

namespace RFTCC = Robos_FT_Cpp_Client;

int main(int argc, char** argv)
{
    std::cout << "Robos config path: " << (Utilities::OS::GetCurrentDirectory(__FILE__) +
                                           Utilities::OS::GetPathSep() + "Robos_FT_Cpp_Client_Config.xml").c_str() << std::endl;
    RFTCC::SensorNodePtr pSensorNode = std::make_shared<RFTCC::SensorNode>();
    RFTCC::FactorialNodePtr pFNode = std::make_shared<RFTCC::FactorialNode>();
    RFTCC::ConsoleLoggerNodePtr pLogger = std::make_shared<RFTCC::ConsoleLoggerNode>();

    Robos::Init(Utilities::OS::GetCurrentDirectory(__FILE__) +
                Utilities::OS::GetPathSep() + "Robos_FT_Cpp_Client_Config.xml");
    Robos::Register(pSensorNode);
    Robos::Register(pFNode);
    Robos::Register(pLogger);

    Robos::Start();

    EXPECT_EQ(Robos::IsRunning(), true);

    //std::this_thread::sleep_for(std::chrono::seconds(5));
    Robos::WaitForShutdown();

    Robos::Stop();
    return 0;
}
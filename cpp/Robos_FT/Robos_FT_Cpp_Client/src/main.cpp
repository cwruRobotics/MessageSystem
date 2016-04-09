// SYSTEM INCLUDES
#include <Robos/Robos.hpp>
#include <Utilities/OSUtils.hpp>
#include <iostream>

// C++ PROJECT INCLUDES
#include "Nodes/InputReaderNode.hpp"
#include "Nodes/FactorialNode.hpp"

namespace RFTCC = Robos_FT_Cpp_Client;

int main(int argc, char** argv)
{
    Robos::Init(Utilities::OS::GetCurrentDirectory(__FILE__) + "Robos_FT_Cpp_Client_Config.xml");
    RFTCC::InputReaderNodePtr pIRNode = std::make_shared<RFTCC::InputReaderNode>();
    RFTCC::FactorialNodePtr pFNode = std::make_shared<RFTCC::FactorialNode>();

    Robos::Register(pIRNode);
    Robos::Register(pFNode);

    Robos::Start();

    while(Robos::IsRunning())
    {
    }

    Robos::Stop();
    return 0;
}
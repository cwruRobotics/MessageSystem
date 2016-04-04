// SYSTEM INCLUDES
#include <chrono>
#include <iostream>
#include <thread>
#include <Utilities/OSUtils.hpp>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "Robos/unitTest/TestInitNode.hpp"
#include "Robos/unitTest/TestNode.hpp"
#include "Robos/unitTest/TestCountInitNode.hpp"
#include "Robos/unitTest/CountTestNode.hpp"
#include "Robos/Robos.hpp"

namespace OSUtils = Utilities::OS;

namespace Robos
{
namespace Tests
{

    TEST_CASE("Testing Robos::Init() and Robos::Stop()", "[Robos_unit]")
    {
        std::cout << "Executing Robos_unit [" << 1 << "]" << std::endl;
        REQUIRE( Init(OSUtils::GetCurrentDirectory(__FILE__) + OSUtils::GetPathSep() + "TestEngineConfig.xml") );
        REQUIRE( Stop() );
    }

    TEST_CASE("Testing Robos::Register()", "[Robos_unit]")
    {
        std::cout << "Executing Robos_unit [" << 2 << "]" << std::endl;

        REQUIRE( Init(OSUtils::GetCurrentDirectory(__FILE__) + OSUtils::GetPathSep() + "TestEngineConfig.xml") );

        auto pTestInitNode = std::make_shared<TestInitNode>();
        auto pTestNode = std::make_shared<TestNode>();
        REQUIRE( Register(pTestInitNode) );
        REQUIRE( Register(pTestNode) );

        REQUIRE( Stop() );
    }

    TEST_CASE("Testing Robos::Start()", "[Robos_unit]")
    {
        std::cout << "Executing Robos_unit [" << 3 << "]" << std::endl;
        REQUIRE( Init(OSUtils::GetCurrentDirectory(__FILE__) + OSUtils::GetPathSep() + "TestEngineConfig.xml") );

        int count = 0;
        auto pTestInitNode = std::make_shared<TestCountInitNode>(&count);
        auto pCountTestNode = std::make_shared<CountTestNode>();

        REQUIRE( Register(pTestInitNode) );
        REQUIRE( Register(pCountTestNode) );
        Start();

        std::this_thread::sleep_for(std::chrono::seconds(2));
        REQUIRE( count == 1);

        REQUIRE( Stop() );
        
    }

} // end of namespace Tests
} // end of namespace Robos

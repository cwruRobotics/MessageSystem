// SYSTEM INCLUDES
#include <chrono>
#include <iostream>
#include <Utilities/OSUtils.hpp>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "Async/unitTest/TestUtilities.hpp"

#include "Async/Async.hpp"
#include "Async/AsyncExecution.hpp"
#include "Async/SettlementStates.hpp"
#include "Async/Promise.hpp"


namespace OSUtils = Utilities::OS;

namespace Async
{
namespace Tests
{

    TEST_CASE("Testing SimpleContinuation of Promises", "[SimpleContinuation_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("SimpleContinuation_unit test 1");
        std::string configFilePath = OSUtils::GetCurrentDirectory(__FILE__) +
            OSUtils::GetPathSep() + "TestEngineConfig.xml";
        Start(configFilePath);

        std::string schedulerId = "TestScheduler";
        PromisePtr<int> pPromise = Execute<int>([]() -> int
        {
            return 10;
        }, schedulerId);
        PromisePtr<int> pSuccessor = pPromise->Then<int>([](int a) -> int
        {
            return a + 5;
        }, schedulerId);

        std::cout << "\tSleeping for 2 seconds to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));

        REQUIRE( pPromise->GetState() == States::SettlementState::SUCCESS );
        REQUIRE( pPromise->GetResult() == 10 );

        REQUIRE( pSuccessor->GetState() == States::SettlementState::SUCCESS );
        REQUIRE( pSuccessor->GetResult() == 15 );

        Stop();
    }

    TEST_CASE("Creating SimpleContinuations guarenteeing that Then() is called while Promise is executing",
        "[SimpleContinuation_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("SimpleContinuation_unit test 2");
        std::string configFilePath = OSUtils::GetCurrentDirectory(__FILE__) +
            OSUtils::GetPathSep() + "TestEngineConfig.xml";
        Start(configFilePath);

        std::string schedulerId = "TestScheduler";
        PromisePtr<int> pPromise = Execute<int>([]() -> int
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return 10;
        }, schedulerId);

        PromisePtr<int> pSuccessor = pPromise->Then<int>([](int a) -> int
        {
            return a + 5;
        }, schedulerId);

        while ( pPromise->GetState() == States::SettlementState::PENDING )
        {
            std::cout << "\tSleeping for 1 second to allow execution" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        REQUIRE( pPromise->GetState() == States::SettlementState::SUCCESS );
        REQUIRE( pPromise->GetResult() == 10 );

        while( pSuccessor->GetState() == States::SettlementState::PENDING )
        {
            std::cout << "\tSleeping for 1 second to allow execution" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        REQUIRE( pSuccessor->GetState() == States::SettlementState::SUCCESS );
        REQUIRE( pSuccessor->GetResult() == 15 );

        Stop();
    }

} // end of namespace Tests
} // end of namespace Async

// SYSTEM INCLUDES
#include <chrono>
#include <iostream>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "FuturesFramework/unitTest/MockScheduler.hpp"
#include "FuturesFramework/SettlementStates.hpp"
#include "FuturesFramework/Promise.hpp"

namespace FuturesFramework
{
namespace Tests
{

    TEST_CASE("Testing Promise default constructor", "[Promise_unit]")
    {
        Promise<int(int)> p;

        REQUIRE( !p.PreconditionsMet() );
        REQUIRE( p.GetError() == nullptr );
        REQUIRE( p.GetState() == States::SettlementState::PENDING );
    }

    TEST_CASE("Testing Promise PreconditionsMet()", "[Promise_unit]")
    {
        Promise<int(int)> p;

        p.AttachMainFunction([](int a) -> int
        {
            return a + 5;
        });
        p.GiveArgs(5);

        REQUIRE( p.PreconditionsMet() );
        REQUIRE( p.GetState() == States::SettlementState::PENDING );
    }

    TEST_CASE("Testing Promise Execution on Schedulers", "[Promise_unit]")
    {
        PromisePtr<int, int> pPromise =
            std::make_shared<Promise<int(int)> >();
        MockSchedulerPtr pMockScheduler =
            std::make_shared<MockScheduler>();

        pPromise->AttachMainFunction([](int a) -> int
        {
            return a + 5;
        });

        pPromise->Schedule(pMockScheduler);

        std::cout << "Sleeping for 1 second to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        REQUIRE( pPromise->GetState() ==
            States::SettlementState::PENDING );

        pPromise->GiveArgs(5);

        std::cout << "Sleeping for 1 second to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        REQUIRE( pPromise->GetState() ==
            States::SettlementState::SUCCESS );

        REQUIRE( pPromise->GetResult() == 10 );
    }

    TEST_CASE("Testing Promise Execution on Schedulers that Throw", "[Promise_unit]")
    {
        PromisePtr<int, int> pPromise =
            std::make_shared<Promise<int(int)> >();
        MockSchedulerPtr pMockScheduler =
            std::make_shared<MockScheduler>();

        pPromise->AttachMainFunction([](int a) -> int
        {
            throw std::exception("Blah");
        });

        pPromise->Schedule(pMockScheduler);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        REQUIRE( pPromise->GetState() ==
            States::SettlementState::PENDING );

        pPromise->GiveArgs(5);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        REQUIRE( pPromise->GetState() ==
            States::SettlementState::FAILURE );

        REQUIRE( pPromise->GetError() != nullptr );

        try
        {
            pPromise->GetResult();

            // this line should not be executed.
            REQUIRE( false );
        }
        catch(...)
        {
            REQUIRE( true );
        }
    }

} // end of namespace Tests
} // end of namespace FuturesFramework

// SYSTEM INCLUDES


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
        PromisePtr<int, int> pPromise = std::make_shared<Promise<int(int)> >();
        MockSchedulerPtr pMockScheduler =
            std::make_shared<MockScheduler>();

        pPromise->AttachMainFunction([](int a) -> int
        {
            return a + 5;
        });

        pPromise->Schedule(pMockScheduler);

        REQUIRE( pMockScheduler->
            ExecuteWorkItem(pPromise->GetId()) );
        REQUIRE( pPromise->GetState() == States::SettlementState::PENDING );

        pPromise->GiveArgs(5);
        REQUIRE( pMockScheduler->
            ExecuteWorkItem(pPromise->GetId()) );
        REQUIRE( pPromise->GetState() == States::SettlementState::SUCCESS );

        REQUIRE( pPromise->GetResult() == 10 );
    }

} // end of namespace Tests
} // end of namespace FuturesFramework

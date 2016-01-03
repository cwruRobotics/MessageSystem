// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "FuturesFramework/unitTest/MockScheduler.hpp"
#include "FuturesFramework/Result.hpp"
#include "FuturesFramework/SettlementStates.hpp"
#include "FuturesFramework/ContinuableWorkItem.hpp"

namespace FuturesFramework
{
namespace Tests
{

    TEST_CASE("Testing ContinuableWorkItem default constructor",
        "[ContinuableWorkItem_unit]")
    {
        ContinuableWorkItem workItem;

        REQUIRE( workItem.GetState() ==
            States::SettlementState::PENDING );
        REQUIRE( !workItem.IsCurrentlyExecuting() );
    }

    TEST_CASE("Testing ContinuableWorkItem SetSuccess()",
        "[ContinuableWorkItem_unit]")
    {
        ContinuableWorkItem workItem;

        workItem.SetSuccess();
        REQUIRE( workItem.GetState() ==
            States::SettlementState::SUCCESS );
    }

    TEST_CASE("Testing ContinuableWorkItem SetFailure()",
        "[ContinuableWorkItem_unit]")
    {
        ContinuableWorkItem workItem;

        workItem.SetFailure();
        REQUIRE( workItem.GetState() ==
            States::SettlementState::FAILURE );
    }

    TEST_CASE("Testing ContinuableWorkItem execution",
        "[ContinuableWorkItem_unit]")
    {
        MockSchedulerPtr pMockScheduler =
            std::make_shared<MockScheduler>();

        ContinuableWorkItemPtr pSuccessWorkItem =
            std::make_shared<ContinuableWorkItem>();
        ContinuableWorkItemPtr pFailWorkItem =
            std::make_shared<ContinuableWorkItem>();

        auto pSuccessFunction = [&]() -> Types::Result_t
        {
            REQUIRE( pSuccessWorkItem->IsCurrentlyExecuting() );
            return Types::Result_t::SUCCESS;
        };
        auto pFailFunction = [&]() -> Types::Result_t
        {
            REQUIRE( pFailWorkItem->IsCurrentlyExecuting() );
            return Types::Result_t::FAILURE;
        };

        pSuccessWorkItem->AttachMainFunction(pSuccessFunction);
        pFailWorkItem->AttachMainFunction(pFailFunction);

        REQUIRE( pSuccessWorkItem->Schedule(pMockScheduler) ==
            Types::Result_t::SUCCESS );
        REQUIRE( pFailWorkItem->Schedule(pMockScheduler) ==
            Types::Result_t::SUCCESS );

        REQUIRE( pMockScheduler->
            ExecuteWorkItem(pSuccessWorkItem->GetId()) );
        REQUIRE( pMockScheduler->
            ExecuteWorkItem(pFailWorkItem->GetId()) );

        REQUIRE( pMockScheduler->
            DetachWorkItem(pSuccessWorkItem->GetId()) );
        REQUIRE( pMockScheduler->
            DetachWorkItem(pFailWorkItem->GetId()) );
    }

} // end of namespace Tests
} // end of namespace FuturesFramework

// SYSTEM INCLUDES
#include <chrono>
#include <iostream>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "Async/unitTest/MockScheduler.hpp"
#include "Async/Result.hpp"
#include "Async/SettlementStates.hpp"
#include "Async/ContinuableWorkItem.hpp"

namespace Async
{
namespace Tests
{

    TEST_CASE("Testing ContinuableWorkItem default constructor", "[ContinuableWorkItem_unit]")
    {
        ContinuableWorkItem workItem;

        REQUIRE( workItem.GetState() ==
            States::SettlementState::PENDING );
        REQUIRE( !workItem.IsCurrentlyExecuting() );
    }

    TEST_CASE("Testing ContinuableWorkItem SetSuccess()", "[ContinuableWorkItem_unit]")
    {
        ContinuableWorkItem workItem;

        workItem.SetSuccess();
        REQUIRE( workItem.GetState() ==
            States::SettlementState::SUCCESS );
    }

    TEST_CASE("Testing ContinuableWorkItem SetFailure()", "[ContinuableWorkItem_unit]")
    {
        ContinuableWorkItem workItem;

        workItem.SetFailure();
        REQUIRE( workItem.GetState() ==
            States::SettlementState::FAILURE );
    }

    TEST_CASE("Testing ContinuableWorkItem execution", "[ContinuableWorkItem_unit]")
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
            throw std::exception();
        };

        pSuccessWorkItem->AttachMainFunction(pSuccessFunction);
        pFailWorkItem->AttachMainFunction(pFailFunction);

        REQUIRE( pSuccessWorkItem->Schedule(pMockScheduler) ==
            Types::Result_t::SUCCESS );
        REQUIRE( pFailWorkItem->Schedule(pMockScheduler) ==
            Types::Result_t::SUCCESS );

        std::cout << "Sleeping for 1 second to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        REQUIRE( pSuccessWorkItem->GetStateAsString().compare("Done") == 0 );
        REQUIRE( pFailWorkItem->GetStateAsString().compare("Done") == 0 );

    }

} // end of namespace Tests
} // end of namespace Async

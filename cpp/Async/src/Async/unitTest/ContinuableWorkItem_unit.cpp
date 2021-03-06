// SYSTEM INCLUDES
#include <chrono>
#include <iostream>
#include <vector>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "Async/unitTest/MockScheduler.hpp"
#include "Async/unitTest/TestUtilities.hpp"
#include "Async/Result.hpp"
#include "Async/SettlementStates.hpp"
#include "Async/ContinuableWorkItem.hpp"

namespace Async
{
namespace Tests
{

    TEST_CASE("Testing ContinuableWorkItem default constructor", "[ContinuableWorkItem_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("ContinuableWorkItem_unit test 1");
        ContinuableWorkItem workItem;

        REQUIRE( workItem.GetState() == States::SettlementState::PENDING );
        REQUIRE( !workItem.IsCurrentlyExecuting() );
    }

    TEST_CASE("Testing ContinuableWorkItem SetSuccess()", "[ContinuableWorkItem_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("ContinuableWorkItem_unit test 2");
        ContinuableWorkItem workItem;

        workItem.SetSuccess();
        REQUIRE( workItem.GetState() == States::SettlementState::SUCCESS );
    }

    TEST_CASE("Testing ContinuableWorkItem SetFailure()", "[ContinuableWorkItem_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("ContinuableWorkItem_unit test 3");
        ContinuableWorkItem workItem;

        workItem.SetFailure();
        REQUIRE( workItem.GetState() == States::SettlementState::FAILURE );
    }

    TEST_CASE("Testing ContinuableWorkItem execution", "[ContinuableWorkItem_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("ContinuableWorkItem_unit test 4");
        MockSchedulerPtr pMockScheduler = std::make_shared<MockScheduler>(Utilities::GetDefaultSchedulerConfig());

        ContinuableWorkItemPtr pSuccessWorkItem = std::make_shared<ContinuableWorkItem>();
        ContinuableWorkItemPtr pFailWorkItem = std::make_shared<ContinuableWorkItem>();

        pSuccessWorkItem->AttachMainFunction([&]() -> Types::Result_t
        {
            return Types::Result_t::SUCCESS;
        });
        pFailWorkItem->AttachMainFunction([&]() -> Types::Result_t
        {
            throw std::exception();
        });

        REQUIRE( pSuccessWorkItem->Schedule(pMockScheduler) == Types::Result_t::SUCCESS );
        REQUIRE( pFailWorkItem->Schedule(pMockScheduler) == Types::Result_t::SUCCESS );

        std::cout << "\tSleeping for 1 second to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        REQUIRE( pSuccessWorkItem->GetStateAsString().compare("Done") == 0 );
        REQUIRE( pFailWorkItem->GetStateAsString().compare("Done") == 0 );

    }

} // end of namespace Tests
} // end of namespace Async

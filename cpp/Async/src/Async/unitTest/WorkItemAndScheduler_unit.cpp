
// SYSTEM INCLUDES
#include <chrono>
#include <iostream>
#include <vector>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "Async/JobPriorities.hpp"
#include "Async/unitTest/MockScheduler.hpp"
#include "Async/unitTest/TestUtilities.hpp"
#include "Async/WorkItemStates.hpp"
#include "Async/Result.hpp"
#include "Async/unitTest/MockWorkItem.hpp"

namespace Async
{
namespace Tests
{

    TEST_CASE("Testing Scheduler default constructor", "[Scheduler_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("Scheduler_unit test 1");
        MockScheduler scheduler(Utilities::GetDefaultSchedulerConfig());

        REQUIRE( scheduler.GetCurrentWorkItemId() == 0 );
        REQUIRE( scheduler.GetThreadMap().size() == 3 );

    }

    TEST_CASE("Testing Scheduling Work Items", "[Scheduler_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("Scheduler_unit test 2");
        MockSchedulerPtr pMockScheduler =
            std::make_shared<MockScheduler>(Utilities::GetDefaultSchedulerConfig());

        int numberOfWorkItemsToSchedule = 15;
        uint64_t currentWorkItemId = 0;
        std::vector<uint64_t> idVector;
        // test adding a bunch of work Items
        for (int i = 0; i < numberOfWorkItemsToSchedule; ++i)
        {
            MockWorkItemPtr pWorkItem =
                std::make_shared<MockWorkItem>();
            pWorkItem->AttachMainFunction([]() -> States::WorkItemState
            {
                return States::WorkItemState::DONE;
            });
        
            Types::Result_t result = pWorkItem->Schedule(pMockScheduler);
            REQUIRE( result == Types::Result_t::SUCCESS );
            REQUIRE( ++currentWorkItemId == pWorkItem->GetId() );
            idVector.push_back(pWorkItem->GetId());

        }

    }

    TEST_CASE("Testing Executing Work Items on Schedulers with Successful functions",
        "[WorkItemAndScheduler_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("WorkItemAndScheduler_unit test 3");
        MockSchedulerPtr pMockScheduler =
            std::make_shared<MockScheduler>(Utilities::GetDefaultSchedulerConfig());

        // Execute workItem with only a Main Function
        MockWorkItemPtr pWorkItem =
            std::make_shared<MockWorkItem>();

        auto pFunction = []() -> States::WorkItemState
        {
            return States::WorkItemState::DONE;
        };

        pWorkItem->AttachMainFunction(pFunction);

        REQUIRE( pWorkItem->Schedule(pMockScheduler) ==
            Types::Result_t::SUCCESS );

        std::cout << "\tSleeping for 1 second to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        REQUIRE( pWorkItem->GetStateAsString().compare("Done") == 0 );
        //-------------------------------------------

        // Execute workItem with Main Function AND Posterior Function
        pWorkItem = std::make_shared<MockWorkItem>();
        pWorkItem->AttachMainFunction(pFunction);
        pWorkItem->AttachPosteriorFunction(pFunction);

        REQUIRE( pWorkItem->Schedule(pMockScheduler) ==
            Types::Result_t::SUCCESS );

        std::cout << "\tSleeping for 1 second to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        REQUIRE( pWorkItem->GetStateAsString().compare("Done") == 0 );
        //-------------------------------------------
    }

    TEST_CASE("Testing Executing Work Items on Schedulers with unsuccessful functions",
        "[WorkItemAndScheduler_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("WorkItemAndScheduler_unit test 4");
        // --------------------- setup -----------------------
        MockSchedulerPtr pMockScheduler =
            std::make_shared<MockScheduler>(Utilities::GetDefaultSchedulerConfig());

        std::vector<WorkItemPtr> workVector;

        auto pThrowingFunction = []() -> States::WorkItemState
        {
            throw std::exception();
        };
        auto pSuccessfulFunction = []() -> States::WorkItemState
        {
            return States::WorkItemState::DONE;
        };

        MockWorkItemPtr pDoubleThrowWorkItem =
            std::make_shared<MockWorkItem>();
        MockWorkItemPtr pThrowMainWorkItem =
            std::make_shared<MockWorkItem>();
        MockWorkItemPtr pThrowPostWorkItem =
            std::make_shared<MockWorkItem>();

        pDoubleThrowWorkItem->AttachMainFunction(pThrowingFunction);
        pDoubleThrowWorkItem->AttachPosteriorFunction(pThrowingFunction);

        pThrowMainWorkItem->AttachMainFunction(pThrowingFunction);
        pThrowMainWorkItem->AttachPosteriorFunction(pSuccessfulFunction);

        pThrowPostWorkItem->AttachMainFunction(pSuccessfulFunction);
        pThrowPostWorkItem->AttachPosteriorFunction(pThrowingFunction);

        // ---------------------------------------------------

        // ----------------schedule work items----------------
        pDoubleThrowWorkItem->Schedule(pMockScheduler);
        workVector.push_back(pDoubleThrowWorkItem);

        pThrowMainWorkItem->Schedule(pMockScheduler);
        workVector.push_back(pThrowMainWorkItem);

        pThrowPostWorkItem->Schedule(pMockScheduler);
        workVector.push_back(pThrowPostWorkItem);
        // ----------------------------------------------------

        // -----------------execute work items-----------------
        std::cout << "\tSleeping for 5 seconds to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        // ----------------------------------------------------

        for (unsigned int i = 0; i < workVector.size(); ++i)
        {
            REQUIRE( workVector[i]->GetStateAsString().compare("Done") == 0 );
        }
    }

} // end of namespace Tests
} // end of namespace Async

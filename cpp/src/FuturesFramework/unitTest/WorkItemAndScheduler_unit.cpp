
// SYSTEM INCLUDES
#include <chrono>
#include <iostream>
#include <vector>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "FuturesFramework/unitTest/MockScheduler.hpp"
#include "FuturesFramework/Result.hpp"
#include "FuturesFramework/WorkItem.hpp"

namespace FuturesFramework
{
namespace Tests
{

    TEST_CASE("Testing Scheduler default constructor", "[Scheduler_unit]")
    {
        MockScheduler scheduler;

        REQUIRE( scheduler.GetCurrentWorkItemId() == 0 );
        REQUIRE( scheduler.GetWorkItemMap().size() == 0 );
        REQUIRE( scheduler.GetThreadMap().size() == 3 );

    }

    TEST_CASE("Testing Scheduling and Detaching Work Items",
        "[Scheduler_unit]")
    {
        MockSchedulerPtr pMockScheduler =
            std::make_shared<MockScheduler>();

        int numberOfWorkItemsToSchedule = 15;
        uint64_t currentWorkItemId = 0;
        std::vector<uint64_t> idVector;
        // test adding a bunch of work Items
        for (int i = 0; i < numberOfWorkItemsToSchedule; ++i)
        {
            WorkItemPtr pWorkItem =
                std::make_shared<WorkItem>();
            pWorkItem->AttachMainFunction([]() -> Types::Result_t
            {
                return Types::Result_t::SUCCESS;
            });
        
            Types::Result_t result = pWorkItem->Schedule(pMockScheduler);
            REQUIRE( result == Types::Result_t::SUCCESS );
            REQUIRE( ++currentWorkItemId == pWorkItem->GetId() );
            idVector.push_back(pWorkItem->GetId());

            uint64_t id = pWorkItem->GetId();
            auto index = pMockScheduler->GetWorkItemMap().find(id);
            if (index == pMockScheduler->GetWorkItemMap().end())
            {
                REQUIRE( false );
            }
            REQUIRE( id == index->second->GetId() );
        }

        // check all work Items are still in the map
        REQUIRE( pMockScheduler->GetWorkItemMap().size() ==
            numberOfWorkItemsToSchedule );

        for (int i = 0; i < numberOfWorkItemsToSchedule; ++i)
        {
            REQUIRE( pMockScheduler->DetachWorkItem(idVector[i]) );
        }

        REQUIRE( pMockScheduler->GetWorkItemMap().size() == 0 );
    }

    TEST_CASE("Testing Executing Work Items on Schedulers with Successful functions",
        "[WorkItemAndScheduler_unit]")
    {
        MockSchedulerPtr pMockScheduler =
            std::make_shared<MockScheduler>();

        // Execute workItem with only a Main Function
        WorkItemPtr pWorkItem =
            std::make_shared<WorkItem>();

        auto pFunction = []() -> Types::Result_t
        {
            return Types::Result_t::SUCCESS;
        };

        pWorkItem->AttachMainFunction(pFunction);

        REQUIRE( pWorkItem->Schedule(pMockScheduler) ==
            Types::Result_t::SUCCESS );

        std::cout << "Sleeping for 1 second to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        REQUIRE( pWorkItem->GetStateAsString().compare("Done") == 0 );
        //-------------------------------------------

        // Execute workItem with Main Function AND Posterior Function
        pWorkItem = std::make_shared<WorkItem>();
        pWorkItem->AttachMainFunction(pFunction);
        pWorkItem->AttachPosteriorFunction(pFunction);

        REQUIRE( pWorkItem->Schedule(pMockScheduler) ==
            Types::Result_t::SUCCESS );

        std::cout << "Sleeping for 1 second to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        REQUIRE( pWorkItem->GetStateAsString().compare("Done") == 0 );
        //-------------------------------------------
    }

    TEST_CASE("Testing Executing Work Items on Schedulers with unsuccessful functions",
        "[WorkItemAndScheduler_unit]")
    {
        // --------------------- setup -----------------------
        MockSchedulerPtr pMockScheduler =
            std::make_shared<MockScheduler>();

        std::vector<WorkItemPtr> workVector;

        auto pThrowingFunction = []() -> Types::Result_t
        {
            throw std::exception("Dummy exception");
        };
        auto pSuccessfulFunction = []() -> Types::Result_t
        {
            return Types::Result_t::SUCCESS;
        };

        WorkItemPtr pDoubleThrowWorkItem =
            std::make_shared<WorkItem>();
        WorkItemPtr pThrowMainWorkItem =
            std::make_shared<WorkItem>();
        WorkItemPtr pThrowPostWorkItem =
            std::make_shared<WorkItem>();

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
        std::cout << "Sleeping for 5 seconds to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        // ----------------------------------------------------

        for (unsigned int i = 0; i < workVector.size(); ++i)
        {
            REQUIRE( workVector[i]->GetStateAsString().compare("Done") == 0 );
            REQUIRE( pMockScheduler->DetachWorkItem(workVector[i]->GetId()) );
        }
    }

} // end of namespace Tests
} // end of namespace FuturesFramework

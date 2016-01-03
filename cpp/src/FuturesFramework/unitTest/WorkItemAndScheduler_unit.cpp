
// SYSTEM INCLUDES
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
        REQUIRE( scheduler.GetThreadMap().size() == 0 );

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

        REQUIRE( pMockScheduler->ExecuteWorkItem(pWorkItem->GetId()) );
        //-------------------------------------------

        // Execute workItem with Main Function AND Posterior Function
        pWorkItem = std::make_shared<WorkItem>();
        pWorkItem->AttachMainFunction(pFunction);
        pWorkItem->AttachPosteriorFunction(pFunction);

        REQUIRE( pWorkItem->Schedule(pMockScheduler) ==
            Types::Result_t::SUCCESS );
        REQUIRE( pMockScheduler->ExecuteWorkItem(pWorkItem->GetId()) ) ;
        //-------------------------------------------
    }

    TEST_CASE("Testing Executing Work Items on Schedulers with unsuccessful functions",
        "[WorkItemAndScheduler_unit]")
    {
        // --------------------- setup -----------------------
        MockSchedulerPtr pMockScheduler =
            std::make_shared<MockScheduler>();

        std::vector<uint64_t> idVector;

        auto pThrowingFunction = []() -> Types::Result_t
        {
            throw std::exception("Dummy exception");
        };
        auto pUnsuccessfulFunction = []() -> Types::Result_t
        {
            return Types::Result_t::FAILURE;
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
        WorkItemPtr pDoubleFailWorkItem =
            std::make_shared<WorkItem>();
        WorkItemPtr pFailMainWorkItem =
            std::make_shared<WorkItem>();
        WorkItemPtr pFailPostWorkItem =
            std::make_shared<WorkItem>();

        pDoubleThrowWorkItem->AttachMainFunction(pThrowingFunction);
        pDoubleThrowWorkItem->AttachPosteriorFunction(pThrowingFunction);

        pThrowMainWorkItem->AttachMainFunction(pThrowingFunction);
        pThrowMainWorkItem->AttachPosteriorFunction(pSuccessfulFunction);

        pThrowPostWorkItem->AttachMainFunction(pSuccessfulFunction);
        pThrowPostWorkItem->AttachPosteriorFunction(pThrowingFunction);

        pDoubleFailWorkItem->AttachMainFunction(pUnsuccessfulFunction);
        pDoubleFailWorkItem->AttachPosteriorFunction(pUnsuccessfulFunction);

        pFailMainWorkItem->AttachMainFunction(pUnsuccessfulFunction);
        pFailMainWorkItem->AttachPosteriorFunction(pSuccessfulFunction);

        pFailPostWorkItem->AttachMainFunction(pSuccessfulFunction);
        pFailPostWorkItem->AttachPosteriorFunction(pUnsuccessfulFunction);
        // ---------------------------------------------------

        // ----------------schedule work items----------------
        pDoubleThrowWorkItem->Schedule(pMockScheduler);
        idVector.push_back(pDoubleThrowWorkItem->GetId());

        pThrowMainWorkItem->Schedule(pMockScheduler);
        idVector.push_back(pThrowMainWorkItem->GetId());

        pThrowPostWorkItem->Schedule(pMockScheduler);
        idVector.push_back(pThrowPostWorkItem->GetId());

        pDoubleFailWorkItem->Schedule(pMockScheduler);
        idVector.push_back(pDoubleFailWorkItem->GetId());

        pFailMainWorkItem->Schedule(pMockScheduler);
        idVector.push_back(pFailMainWorkItem->GetId());

        pFailPostWorkItem->Schedule(pMockScheduler);
        idVector.push_back(pFailPostWorkItem->GetId());
        // ----------------------------------------------------

        // -----------------execute work items-----------------
        REQUIRE( pMockScheduler->
            ExecuteWorkItem(pDoubleThrowWorkItem->GetId()) );
        REQUIRE( pDoubleThrowWorkItem->GetException() != nullptr );
    
        REQUIRE( pMockScheduler->
            ExecuteWorkItem(pThrowMainWorkItem->GetId()) );
        REQUIRE( pThrowMainWorkItem->GetException() != nullptr );

        REQUIRE( pMockScheduler->
            ExecuteWorkItem(pThrowPostWorkItem->GetId()) );
        REQUIRE( pThrowPostWorkItem->GetException() == nullptr );

        REQUIRE( pMockScheduler->
            ExecuteWorkItem(pDoubleFailWorkItem->GetId()) );
        REQUIRE( pDoubleFailWorkItem->GetException() == nullptr );

        REQUIRE( pMockScheduler->
            ExecuteWorkItem(pFailMainWorkItem->GetId()) );
        REQUIRE( pFailMainWorkItem->GetException() == nullptr );

        REQUIRE( pMockScheduler->
            ExecuteWorkItem(pFailPostWorkItem->GetId()) );
        REQUIRE( pFailPostWorkItem->GetException() == nullptr );
        // ----------------------------------------------------

        for (unsigned int i = 0; i < idVector.size(); ++i)
        {
            REQUIRE( pMockScheduler->DetachWorkItem(idVector[i]) );
        }
    }

} // end of namespace Tests
} // end of namespace FuturesFramework

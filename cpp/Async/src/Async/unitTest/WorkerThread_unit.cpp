
// SYSTEM INCLUDES
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "Async/unitTest/TestUtilities.hpp"
#include "Async/JobPriorities.hpp"
#include "Async/ConcurrencyStates.hpp"
#include "Async/WorkerThread.hpp"
#include "Async/Scheduler.hpp"

#include "Async/Result.hpp"
#include "Async/WorkItemStates.hpp"
#include "Async/unitTest/MockWorkItem.hpp"

#include "Async/SettlementStates.hpp"
//#include "Async/ContinuableWorkItem.hpp"

#include "Async/Promise.hpp"

namespace Async
{
namespace Tests
{

    TEST_CASE("Testing WorkerThread default constructor and Shutdown()",
        "[WorkerThread_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("WorkerThread_unit test 1");
        // construct an object
        Concurrency::WorkerThread wThread;

        // nothing has been executed...so inner thread should be IDLE.
        REQUIRE( wThread.GetState() == States::ConcurrencyState::IDLE );

        wThread.Join();

        REQUIRE( wThread.GetState() == States::ConcurrencyState::DONE );
    }

    TEST_CASE("Testing default WorkerThread destructor", "[WorkerThread_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("WorkerThread_unit test 2");
        try
        {
            // construct an object
            Concurrency::WorkerThread wThread;

            // nothing has been executed...so inner thread should be IDLE
            REQUIRE( wThread.GetState() == States::ConcurrencyState::IDLE );
        }
        catch(...)
        {
            REQUIRE( false );
        }
    }

    TEST_CASE("Testing Queue and Execution with WorkItems",
        "[WorkerThread_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("WorkerThread_unit test 3");
        MockWorkItemPtr pWorkItem = std::make_shared<MockWorkItem>();

        Concurrency::WorkerThreadPtr pWorkerThread =
            std::make_shared<Concurrency::WorkerThread>();

        pWorkItem->AttachMainFunction([&]() -> States::WorkItemState
        {
            REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::BUSY );
            return States::WorkItemState::DONE;
        });

        REQUIRE( pWorkerThread->Queue(pWorkItem) == Types::Result_t::SUCCESS );

        std::cout << "\tSleeping for 3 seconds to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        REQUIRE( pWorkItem->GetException() == nullptr );
        REQUIRE( pWorkItem->GetStateAsString().compare("Done") == 0 );
        REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::IDLE );

    }

    TEST_CASE("Testing Queuing and Execution of Unsuccessful WorkItem",
        "[WorkerThread_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("WorkerThread_unit test 4");
        MockWorkItemPtr pWorkItem = std::make_shared<MockWorkItem>();

        Concurrency::WorkerThreadPtr pWorkerThread =
            std::make_shared<Concurrency::WorkerThread>();

        pWorkItem->AttachMainFunction([&]() -> States::WorkItemState
        {
            throw std::exception();
        });

        REQUIRE( pWorkerThread->Queue(pWorkItem) == Types::Result_t::SUCCESS );

        std::cout << "\tSleeping for 1 second to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        REQUIRE( pWorkItem->GetException() != nullptr );
        REQUIRE( pWorkItem->GetStateAsString().compare("Done") == 0 );
        REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::IDLE );
    }

    TEST_CASE("Testing Queuing and Execution of 5 WorkItems interleaved",
        "[WorkerThread_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("WorkerThread_unit test 5");
        MockWorkItemPtr pWorkItem = std::make_shared<MockWorkItem>();

        Concurrency::WorkerThreadPtr pWorkerThread =
            std::make_shared<Concurrency::WorkerThread>();

        // attach function that will Queue 4 more WorkItems for execution
        pWorkItem->AttachMainFunction([&]() -> States::WorkItemState
        {
            REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::BUSY );

            MockWorkItemPtr pWorkItem2 = std::make_shared<MockWorkItem>();
            // attach function that will Queue 3 more WorkItems for execution
            pWorkItem2->AttachMainFunction([&]() -> States::WorkItemState
            {
                REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::BUSY );

                MockWorkItemPtr pWorkItem3 = std::make_shared<MockWorkItem>();
                // attach function that will Queue 2 more WorkItems for execution
                pWorkItem3->AttachMainFunction([&]() -> States::WorkItemState
                {
                    REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::BUSY );

                    MockWorkItemPtr pWorkItem4 = std::make_shared<MockWorkItem>();
                    // attach function that will Queue 1 more WorkItem for execution
                    pWorkItem4->AttachMainFunction([&]() -> States::WorkItemState
                    {
                        REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::BUSY );

                        MockWorkItemPtr pWorkItem5 = std::make_shared<MockWorkItem>();
                        // return Success
                        pWorkItem5->AttachMainFunction([]() -> States::WorkItemState
                        {
                            return States::WorkItemState::DONE;
                        });

                        REQUIRE( pWorkerThread->Queue(pWorkItem5) == Types::Result_t::SUCCESS );

                        return States::WorkItemState::DONE;
                       
                    });

                    REQUIRE( pWorkerThread->Queue(pWorkItem4) == Types::Result_t::SUCCESS );

                    return States::WorkItemState::DONE;
                });
                REQUIRE( pWorkerThread->Queue(pWorkItem3) == Types::Result_t::SUCCESS );

                return States::WorkItemState::DONE;

            });
            REQUIRE( pWorkerThread->Queue(pWorkItem2) == Types::Result_t::SUCCESS );

            return States::WorkItemState::DONE;
        });

        REQUIRE( pWorkerThread->Queue(pWorkItem) == Types::Result_t::SUCCESS );

        std::cout << "\tSleeping for 1 second to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::IDLE );
    }

    TEST_CASE("Stress test Queuing and Execution of WorkItems", "[WorkerThread_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("WorkerThread_unit test 6");
        int stressLimit = 100;
        std::vector<WorkItemPtr> queuedWorkItems;

        Concurrency::WorkerThreadPtr pWorkerThread =
            std::make_shared<Concurrency::WorkerThread>();

        for(int i = 0; i < stressLimit; ++i)
        {
            MockWorkItemPtr pWorkItem = std::make_shared<MockWorkItem>();
            pWorkItem->AttachMainFunction([]() -> States::WorkItemState
            {
                return States::WorkItemState::DONE;
            });

            queuedWorkItems.push_back(pWorkItem);
            REQUIRE( pWorkerThread->Queue(pWorkItem) == Types::Result_t::SUCCESS );
        }

        std::cout << "\tSleeping for 4 seconds to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(4));

        for(int i = 0; i < stressLimit; ++i)
        {
            REQUIRE( queuedWorkItems[i]->GetStateAsString().compare("Done") == 0 );
        }

        REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::IDLE );
    }

} // end of namespace Tests
} // end of namespace Async

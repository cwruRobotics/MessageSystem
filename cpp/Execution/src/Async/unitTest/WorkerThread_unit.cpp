
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
#include "Async/WorkItem.hpp"

#include "Async/SettlementStates.hpp"
#include "Async/ContinuableWorkItem.hpp"

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
        WorkItemPtr pWorkItem = std::make_shared<WorkItem>();

        Concurrency::WorkerThreadPtr pWorkerThread =
            std::make_shared<Concurrency::WorkerThread>();

        pWorkItem->AttachMainFunction([&]() -> Types::Result_t
        {
            REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::BUSY );
            return Types::Result_t::SUCCESS;
        });

        REQUIRE( pWorkerThread->Queue(pWorkItem) == Types::Result_t::SUCCESS );

        std::cout << "\tSleeping for 3 seconds to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        REQUIRE( pWorkItem->GetException() == nullptr );
        REQUIRE( pWorkItem->GetStateAsString().compare("Done") == 0 );
        REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::IDLE );

    }

    TEST_CASE("Testing Queue and Execution with ContinuableWorkItems",
        "[WorkerThread_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("WorkerThread_unit test 4");
        ContinuableWorkItemPtr pContinuableWorkItem =
            std::make_shared<ContinuableWorkItem>();

        Concurrency::WorkerThreadPtr pWorkerThread =
            std::make_shared<Concurrency::WorkerThread>();

        pContinuableWorkItem->AttachMainFunction([&]() -> Types::Result_t
        {
            REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::BUSY );
            return Types::Result_t::SUCCESS;
        });

        REQUIRE( pWorkerThread->Queue(pContinuableWorkItem) == Types::Result_t::SUCCESS );

        std::cout << "\tSleeping for 3 seconds to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        REQUIRE( pContinuableWorkItem->GetException() == nullptr );
        REQUIRE( pContinuableWorkItem->GetStateAsString().compare("Done") == 0 );
        REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::IDLE );
        
    }

    TEST_CASE("Testing Queuing and Execution of Unsuccessful WorkItem",
        "[WorkerThread_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("WorkerThread_unit test 5");
        WorkItemPtr pWorkItem = std::make_shared<WorkItem>();

        Concurrency::WorkerThreadPtr pWorkerThread =
            std::make_shared<Concurrency::WorkerThread>();

        pWorkItem->AttachMainFunction([&]() -> Types::Result_t
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
        Utilities::WriteUnitTestNameToConsole("WorkerThread_unit test 6");
        WorkItemPtr pWorkItem = std::make_shared<WorkItem>();

        Concurrency::WorkerThreadPtr pWorkerThread =
            std::make_shared<Concurrency::WorkerThread>();

        // attach function that will Queue 4 more WorkItems for execution
        pWorkItem->AttachMainFunction([&]() -> Types::Result_t
        {
            REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::BUSY );

            WorkItemPtr pWorkItem2 = std::make_shared<WorkItem>();
            // attach function that will Queue 3 more WorkItems for execution
            pWorkItem2->AttachMainFunction([&]() -> Types::Result_t
            {
                REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::BUSY );

                WorkItemPtr pWorkItem3 = std::make_shared<WorkItem>();
                // attach function that will Queue 2 more WorkItems for execution
                pWorkItem3->AttachMainFunction([&]() -> Types::Result_t
                {
                    REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::BUSY );

                    WorkItemPtr pWorkItem4 = std::make_shared<WorkItem>();
                    // attach function that will Queue 1 more WorkItem for execution
                    pWorkItem4->AttachMainFunction([&]() -> Types::Result_t
                    {
                        REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::BUSY );

                        WorkItemPtr pWorkItem5 = std::make_shared<WorkItem>();
                        // return Success
                        pWorkItem5->AttachMainFunction([]() -> Types::Result_t
                        {
                            return Types::Result_t::SUCCESS;
                        });

                        REQUIRE( pWorkerThread->Queue(pWorkItem5) == Types::Result_t::SUCCESS );

                        return Types::Result_t::SUCCESS;
                       
                    });

                    REQUIRE( pWorkerThread->Queue(pWorkItem4) == Types::Result_t::SUCCESS );

                    return Types::Result_t::SUCCESS;
                });
                REQUIRE( pWorkerThread->Queue(pWorkItem3) == Types::Result_t::SUCCESS );

                return Types::Result_t::SUCCESS;

            });
            REQUIRE( pWorkerThread->Queue(pWorkItem2) == Types::Result_t::SUCCESS );

            return Types::Result_t::SUCCESS;
        });

        REQUIRE( pWorkerThread->Queue(pWorkItem) == Types::Result_t::SUCCESS );

        std::cout << "\tSleeping for 1 second to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        REQUIRE( pWorkerThread->GetState() == States::ConcurrencyState::IDLE );
    }

    TEST_CASE("Stress test Queuing and Execution of WorkItems", "[WorkerThread_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("WorkerThread_unit test 7");
        int stressLimit = 100;
        std::vector<WorkItemPtr> queuedWorkItems;

        Concurrency::WorkerThreadPtr pWorkerThread =
            std::make_shared<Concurrency::WorkerThread>();

        for(int i = 0; i < stressLimit; ++i)
        {
            WorkItemPtr pWorkItem = std::make_shared<WorkItem>();
            pWorkItem->AttachMainFunction([]() -> Types::Result_t
            {
                return Types::Result_t::SUCCESS;
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
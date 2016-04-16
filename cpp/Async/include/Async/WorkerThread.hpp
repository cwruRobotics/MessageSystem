#pragma once

#ifndef ASYNC_CONCURRENCY_WORKERTHREAD_HPP
#define ASYNC_CONCURRENCY_WORKERTHREAD_HPP

// SYSTEM INCLUDES
#include <atomic>
#include <condition_variable>
#include <queue>
#include <memory>
#include <mutex>
#include <thread>

// C++ PROJECT INCLUDES
#include "Async/ThreadBase.hpp"
#include "Async/JobPriorities.hpp"

namespace Async
{
namespace Concurrency
{

    class WorkerThread : public ThreadBase,
        public std::enable_shared_from_this<WorkerThread>
    {
    private:

        // thread variables
        std::thread                             _thread; // the concurrent thread
        std::condition_variable                 _threadCV; // allows _thread to sleep when queue is empty

        // state variables
        std::atomic<States::ConcurrencyState>   _state; // read for testing / status
        std::atomic<bool>                       _run;      // false when thread is shutting down
                                                           // should this be modeled using test_and_set()?
                                                           // is this racey having this "unprotected"?

        std::mutex                              _queueMutex; // protects queue
        std::queue<ExecutableWorkItemBasePtr>   _queue; // execution queue

    private:

        bool IsQueueEmpty();

    public:

        WorkerThread();

        ~WorkerThread();

        std::thread::id GetId() override;

        States::ConcurrencyState GetState() override;

        Types::Result_t Queue(ExecutableWorkItemBasePtr workItem) override;

        void Stop() override;

        void Join() override;

        Types::Result_t Abort() override;

        void Run() override;


    };

    using WorkerThreadPtr = std::shared_ptr<WorkerThread>;
} // end of namespace Concurrency
} // end of namespace Async

#endif

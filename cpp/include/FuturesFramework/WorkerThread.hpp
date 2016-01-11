#pragma once

#ifndef FUTURESFRAMEWORK_CONCURRENCY_WORKERTHREAD_HPP
#define FUTURESFRAMEWORK_CONCURRENCY_WORKERTHREAD_HPP

// SYSTEM INCLUDES
#include <atomic>
#include <condition_variable>
#include <queue>
#include <memory>
#include <mutex>
#include <thread>

// C++ PROJECT INCLUDES
#include "FuturesFramework/IThread.hpp"

namespace FuturesFramework
{
namespace Concurrency
{

    class WorkerThread : public IThread,
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
        std::queue<IExecutableWorkItemPtr>      _queue; // execution queue
        uint64_t                                _id;

    private:

        bool IsQueueEmpty();

    public:

        WorkerThread(uint64_t id) : _id(id), _state(States::ConcurrencyState::IDLE),
            _queueMutex(), _queue(), _run(true), _threadCV()
        {
            this->_thread = std::thread(&WorkerThread::Run, this);
        }

        ~WorkerThread()
        {
            if (this->_run)
            {
                this->Join();
            }
        }

        std::thread::id GetId() override;

        States::ConcurrencyState GetState() override;

        Types::Result_t Queue(IExecutableWorkItemPtr workItem) override;

        void Join() override;

        Types::Result_t Abort() override;

        void Run() override;


    };

    using WorkerThreadPtr = std::shared_ptr<WorkerThread>;
} // end of namespace Concurrency
} // end of namespace FuturesFramework

#endif

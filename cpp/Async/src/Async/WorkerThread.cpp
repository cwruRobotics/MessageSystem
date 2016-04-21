
// SYSTEM INCLUDES
#include <iostream>

// C++ PROJECT INCLUDES
#include "Async/WorkerThread.hpp"
#include "Async/WorkItem.hpp"

// logging functionality
/**
#include "AsyncLoggingConfig.hpp"
//#include "Logging/ILogger.hpp"
#include "Logging/Factory.hpp"

Logging::LoggerPtr GetStaticCallerLogger(std::string workerThreadId)
{
    static Logging::LoggerPtr pLogger =
        Logging::Factory::MakeLogger(workerThreadId.c_str(),
                                    (Async::Logging::LOGGING_ROOT + workerThreadId + ".txt").c_str());
    return pLogger;
}
*/

namespace Async
{
namespace Concurrency
{

    WorkerThread::WorkerThread() : _thread(), _threadCV(),
        _state(States::ConcurrencyState::IDLE), _run(true),
        _queueMutex(), _queue()
    {
        this->_thread = std::thread(&WorkerThread::Run, this);
    }

    WorkerThread::~WorkerThread()
    {
        if (this->_run)
        {
            this->Join();
        }
    }

    bool WorkerThread::IsQueueEmpty()
    {
        std::lock_guard<std::mutex> queueLock(this->_queueMutex);
        return this->_queue.empty();
    }

    std::thread::id WorkerThread::GetId()
    {
        return this->_thread.get_id();
    }

    States::ConcurrencyState WorkerThread::GetState()
    {
        return this->_state;
    }

    Types::Result_t WorkerThread::Queue(ExecutableWorkItemBasePtr workItem)
    {
        // modifying queue so need to aquire lock on it
        std::unique_lock<std::mutex> queueLock(this->_queueMutex);
        this->_queue.push(workItem);

        // wake up the spawned thread if it is sleeping.
        this->_threadCV.notify_one();
        queueLock.unlock();

        return Types::Result_t::SUCCESS;
    }

    void WorkerThread::Stop()
    {
        this->_run = false;
    }

    void WorkerThread::Join()
    {
        this->_run = false;

        // synchronization! This prevents a algorithmic hole in
        // the second while loop in WorkerThread::Run
        std::unique_lock<std::mutex> queueLock(this->_queueMutex);
        // std::cout << "WorkerThread shutting down...";

        this->_threadCV.notify_all();

        // MUST unlock! Otherwise, if we try and join with the _thread,
        // the _thread is waiting for _queueMutex to be released.
        // if we don't release the lock, we will have a deadlock
        // (this thread will wait for _thread to exit, while _thread is
        // waiting for _queueMutex to be unlocked but this thread possesses
        // a lock on _queueMutex which will never be released)!
        queueLock.unlock();
        if (this->_thread.joinable())
        {
            this->_thread.join();
        }
        this->_state = States::ConcurrencyState::DONE;
        // std::cout << "WorkerThread shut down" << std::endl;

    }

    Types::Result_t WorkerThread::Abort()
    {
        return Types::Result_t::FAILURE;
    }

    // this is the method that the concurrent thread (this->_thread) will run.
    // it will try and get workItems from the queue when there are stuff
    // to be had, otherwise it will go to sleep to be woken when the
    // queue is populated.
    void WorkerThread::Run()
    {
        // the work item we will be executing
        ExecutableWorkItemBasePtr workItem = nullptr;

        while (this->_run)
        {
            std::unique_lock<std::mutex> queueLock(this->_queueMutex);
            while (this->_run && !this->_queue.empty())
            {

                // this is atomic
                this->_state = States::ConcurrencyState::BUSY;

                // lock the queue and retrieve the WorkItem.
                // this is in its own code block because the lock
                // is released only when the object destructs.
                workItem = this->_queue.front();
                this->_queue.pop();
                queueLock.unlock();

                // execute the WorkItem
                // DON'T BLOCK HERE....THIS COULD BE VERY EXPENSIVE
                // BOTH RUNTIME AND RESOURCE WISE
                workItem->Execute();

                queueLock.lock();
                // reschedule if necessary
                if (!workItem->IsDone())
                {
                    this->_queue.push(workItem);
                }
            }
            this->_state = States::ConcurrencyState::IDLE;


            // lock on this thread
            // put the concurrent thread to sleep until it is woken up by
            // a WorkItem being queued.
            if (this->_run)
            {
                this->_threadCV.wait(queueLock);
            }
        }
    }

} // end of namespace Concurrency
} // end of namespace Async

#pragma once

#ifndef FUTURESFRAMEWORK_SCHEDULER_HPP
#define FUTURESFRAMEWORK_SCHEDULER_HPP

// SYSTEM INCLUDES
#include <mutex>
#include <string>

// C++ PROJECT INCLUDES
#include "FuturesFramework/IExecutableWorkItem.hpp"
#include "FuturesFramework/IScheduler.hpp"
#include "FuturesFramework/WorkerThread.hpp"


// thread max
#ifndef THREAD_POOL_MAX
#define THREAD_POOL_MAX 1
#endif


// project namespace
namespace FuturesFramework
{

    // FORWARD DECLARATION
	class Scheduler;
	using SchedulerPtr = std::shared_ptr<Scheduler>;

    // a Scheduler. An instance of this will execute IExecutableWorkItems
    // (for now IWorkItems).
	class Scheduler : public IScheduler,
        public std::enable_shared_from_this<Scheduler>
	{
	private: 

		// global variables
		std::map<uint64_t, IExecutableWorkItemPtr>	            _attachedWorkItems;
		std::mutex								                _mutex;

		// thread pool variables
		std::map<std::thread::id, Concurrency::IThreadPtr>      _threadMap;

		uint64_t								                _currentWorkItemId;
        // Concurrency::IThreadPtr                                 _tempWorkerThread;
        bool                                                    _running;

	protected:

		std::map<uint64_t, IExecutableWorkItemPtr>& GetWorkItemMap() override;

		std::map<std::thread::id, Concurrency::IThreadPtr>& GetThreadMap() override;

		bool DetachWorkItem(uint64_t id) override;

        // std::queue<IExecutableWorkItemPtr>& GetQueue();

        bool ExecuteWorkItem(const uint64_t id);

	public:

		Scheduler() : _attachedWorkItems(), _mutex(),
            _threadMap(), _currentWorkItemId(0), _running(false) // ,
            // _tempWorkerThread(std::make_shared<Concurrency::WorkerThread>())
		{
            // std::lock_guard<std::mutex>(this->_mutex);
            // Concurrency::IThreadPtr pWorkerThread = nullptr;
            // for (int i = 0; i < THREAD_POOL_MAX; ++i)
            // {
            //     pWorkerThread = std::make_shared<Concurrency::WorkerThread>();
            //     this->_threadMap.insert(std::pair<std::thread::id,
            //         Concurrency::IThreadPtr>(pWorkerThread->GetId(), pWorkerThread));
            // }
		}

		virtual ~Scheduler()
		{
            this->Shutdown();
		}

		bool ScheduleWorkItem(IExecutableWorkItemPtr workItem) override;

		void Run() override;

        void Shutdown() override;

        const uint64_t GetCurrentWorkItemId();

	};

} // end of namespace FuturesFramework

#endif // end of header guard

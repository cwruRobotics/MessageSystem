#pragma once

#ifndef ASYNC_SCHEDULER_HPP
#define ASYNC_SCHEDULER_HPP

// SYSTEM INCLUDES
#include <atomic>
#include <mutex>
#include <string>
#include <vector>

// C++ PROJECT INCLUDES
#include "Async/IExecutableWorkItem.hpp"
#include "Async/IScheduler.hpp"
#include "Async/WorkerThread.hpp"

// project namespace
namespace Async
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

		// thread pool variables
		std::map<Types::JobPriority, Concurrency::IThreadPtr>   _threadMap;

		uint64_t								                _currentWorkItemId;
        std::atomic<bool>                                       _running;
        std::string                                             _id;

	protected:

		std::map<Types::JobPriority, Concurrency::IThreadPtr>& GetThreadMap() override;

	public:

		Scheduler(std::vector<Types::JobPriority>& configuration, std::string id="");

		virtual ~Scheduler();

		bool ScheduleWorkItem(IExecutableWorkItemPtr workItem) override;

        void Shutdown() override;

        bool IsRunning() override;

        const uint64_t GetCurrentWorkItemId();

	};

} // end of namespace Async

#endif // end of header guard

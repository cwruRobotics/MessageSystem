#pragma once

#ifndef FUTURESFRAMEWORK_SCHEDULER_HPP
#define FUTURESFRAMEWORK_SCHEDULER_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/IScheduler.hpp"
#include "FuturesFramework/WorkItem.hpp"

// project namespace
namespace FuturesFramework
{

    // FORWARD DECLARATION
	class Scheduler;
	using SchedulerPtr = std::shared_ptr<Scheduler>;

    // a Scheduler. An instance of this will execute IExecutableWorkItems
    // (for now IWorkItems).
	class Scheduler : public IScheduler,
        public virtual std::enable_shared_from_this<Scheduler>
	{
	private:

		// global variables
		std::map<uint64_t, IWorkItemPtr>		_attachedWorkItems;
		std::mutex								_mutex;

		// thread pool variables
		std::map<std::thread::id, std::thread>	_threadMap;

		uint64_t								_currentWorkItemId;

	protected:

		std::map<uint64_t, IWorkItemPtr>& GetWorkItemMap() override;

		std::map<std::thread::id, std::thread>& GetThreadMap() override;

		bool DetachWorkItem(uint64_t id) override;

	public:

		Scheduler() : _attachedWorkItems(), _mutex(), _currentWorkItemId(0)
		{
		}

		virtual ~Scheduler()
		{
		}

		bool ScheduleWorkItem(IWorkItemPtr workItem) override;

		void Run() override;

		bool ExecuteWorkItem(const uint64_t id);

	};

} // end of namespace FuturesFramework

#endif // end of header guard

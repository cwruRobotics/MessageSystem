#pragma once

#ifndef FUTURESFRAMEWORK_ISCHEDULER_HPP
#define FUTURESFRAMEWORK_ISCHEDULER_HPP

// SYSTEM INCLUDES
#include <queue>
#include <map>
#include <memory>
#include <mutex>
#include <thread>

// C++ PROJECT INCLUDES
#include "FuturesFramework/LibraryExport.hpp"
#include "FuturesFramework/IWorkItem.hpp"

// project namespace
namespace FuturesFramework
{

    // FORWARD DECLARATION
	class IScheduler;
	using ISchedulerPtr = std::shared_ptr<IScheduler>;

    // inteface for a Scheduler. A Scheduler will actually
    // execute IExecutableWorkItems. Currently, it accepts
    // IWorkItems, but this will change, as IExecutableWorkItems
    // actually have the Execute() method.
	class FUTURESFRAMEWORK_API IScheduler
	{
	private:

	protected:

		virtual std::map<uint64_t, IWorkItemPtr>& GetWorkItemMap() = 0;

		virtual std::map<std::thread::id, std::thread>& GetThreadMap() = 0;

		virtual bool DetachWorkItem(uint64_t id) = 0;

	public:

		virtual ~IScheduler() = default;

		virtual bool ScheduleWorkItem(IWorkItemPtr workItem) = 0;

		virtual void Run() = 0;

	};

} // end of namespace FuturesFramework

#endif // end of header guard

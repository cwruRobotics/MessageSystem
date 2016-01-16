#pragma once

#ifndef ASYNC_ISCHEDULER_HPP
#define ASYNC_ISCHEDULER_HPP

// SYSTEM INCLUDES
#include <map>
#include <memory>
#include <thread>

// C++ PROJECT INCLUDES
#include "Async/LibraryExport.hpp"
#include "Async/IExecutableWorkItem.hpp"
#include "Async/IThread.hpp"
#include "Async/JobPriorities.hpp"

// project namespace
namespace Async
{

    // FORWARD DECLARATION
	class IScheduler;
	using ISchedulerPtr = std::shared_ptr<IScheduler>;

    // inteface for a Scheduler. A Scheduler will actually
    // execute IExecutableWorkItems. Currently, it accepts
    // IWorkItems, but this will change, as IExecutableWorkItems
    // actually have the Execute() method.
	class ASYNC_API IScheduler
	{
	private:

	protected:

		virtual std::map<uint64_t, IExecutableWorkItemPtr>& GetWorkItemMap() = 0;

		virtual std::map<Types::JobPriority, Concurrency::IThreadPtr>& GetThreadMap() = 0;

		virtual bool DetachWorkItem(uint64_t id) = 0;

	public:

		virtual ~IScheduler() = default;

		virtual bool ScheduleWorkItem(IExecutableWorkItemPtr workItem) = 0;

        virtual void Shutdown() = 0;

        virtual bool IsRunning() = 0;

	};

} // end of namespace Async

#endif // end of header guard

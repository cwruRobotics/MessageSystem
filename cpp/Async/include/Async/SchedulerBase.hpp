#pragma once

#ifndef ASYNC_ISCHEDULER_HPP
#define ASYNC_ISCHEDULER_HPP

// SYSTEM INCLUDES
#include <map>
#include <memory>
#include <thread>

// C++ PROJECT INCLUDES
#include "Async/LibraryExport.hpp"
#include "Async/ExecutableWorkItemBase.hpp"
#include "Async/ThreadBase.hpp"
#include "Async/JobPriorities.hpp"

// project namespace
namespace Async
{

    // FORWARD DECLARATION
	class SchedulerBase;
	using SchedulerBasePtr = std::shared_ptr<SchedulerBase>;

    // inteface for a Scheduler. A Scheduler will actually
    // execute IExecutableWorkItems. Currently, it accepts
    // IWorkItems, but this will change, as IExecutableWorkItems
    // actually have the Execute() method.
	class ASYNC_API SchedulerBase
	{
	protected:

		virtual std::map<Types::JobPriority, Concurrency::ThreadBasePtr>& GetThreadMap() = 0;

	public:

		virtual ~SchedulerBase() = default;

		virtual bool ScheduleWorkItem(ExecutableWorkItemBasePtr workItem) = 0;

        virtual void Shutdown() = 0;

        virtual bool IsRunning() = 0;

	};

} // end of namespace Async

#endif // end of header guard

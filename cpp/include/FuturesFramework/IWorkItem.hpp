#pragma once

#ifndef FUTURESFRAMEWORK_IWORKITEM_HPP
#define FUTURESFRAMEWORK_IWORKITEM_HPP

// #ifndef LOGGING_ON
// #define LOGGING_ON
// #endif

// SYSTEM INCLUDES
#include <cassert>
#include <functional>
#include <memory>
#include <string>
#include <thread>


// C++ PROJECT INCLUDES
// #ifdef LOGGING_ON
// #include "Logging/Factory.hpp"
// #endif

#include "FuturesFramework/SharedLibExport.hpp"
#include "FuturesFramework/Result.hpp"
#include "FuturesFramework/ScheduleRequestStates.hpp"
// #include "FuturesFramework/TypeErasure.hpp"
#include "FuturesFramework/WorkItemStates.hpp"


namespace FuturesFramework
{

	class IScheduler;
	using ISchedulerPtr = std::shared_ptr<IScheduler>;

	using WorkItemFunctionPtr = std::function<Types::Result_t()>;


	class FUTURESFRAMEWORK_API IWorkItem
	{
	public:

		virtual ~IWorkItem() = default;

		virtual void AttachMainFunction(WorkItemFunctionPtr func) = 0;

		virtual void AttachPosteriorFunction(WorkItemFunctionPtr func) = 0;

		virtual Types::Result_t Schedule(ISchedulerPtr scheduler) = 0;

		virtual std::string GetStateString() = 0;

		virtual const uint64_t GetId() = 0;

		virtual std::thread::id GetThreadId() = 0;

	};

	using IWorkItemPtr = std::shared_ptr<IWorkItem>;
}


#endif
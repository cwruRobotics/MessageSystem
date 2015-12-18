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
#include "FuturesFramework/IWorkItem.hpp"


namespace FuturesFramework
{

	class IScheduler;
	using ISchedulerPtr = std::shared_ptr<IScheduler>;

	class IScheduler
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

}

#endif

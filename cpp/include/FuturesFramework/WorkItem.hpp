#pragma once

#ifndef FUTURESFRAMEWORK_WORKITEM_HPP
#define FUTURESFRAMEWORK_WORKITEM_HPP

// SYSTEM INCLUDES
#include <stdexcept>

// C++ PROJECT INCLUDES
#include "FuturesFramework/IExecutable.hpp"
#include "FuturesFramework/IWorkItem.hpp"
#include "FuturesFramework/Scheduler.hpp"
#include "FuturesFramework/WorkItemStateMachine.hpp"

namespace FuturesFramework
{

	class WorkItem : public IWorkItem, public IExecutable, public WorkItemStateMachine,
		public std::enable_shared_from_this<WorkItem>
	{
		friend class Scheduler;
	private:

		uint64_t				_id;
		std::thread::id			_threadId;
		Types::Result_t			_executionResult;
		ISchedulerPtr			_scheduler;
		WorkItemFunctionPtr		_mainFunction;
		WorkItemFunctionPtr		_posteriorFunction;
		std::exception_ptr		_exception;
		bool					_done;

	private:

		void SetId(uint64_t id);

		void SetThreadId(std::thread::id threadId);

		bool IsDone();

	protected:

		void SetException(const std::exception_ptr pException);

		virtual Types::Result_t Execute();

		States::WorkItemState GetInternalState();

		Types::Result_t GetExecutionResult() override;

		void Finish();

	public:

		WorkItem(uint64_t id=0) : WorkItemStateMachine(), _id(id), _threadId(),
			_executionResult(Types::Result_t::UNKNOWN), _mainFunction(nullptr),
			_posteriorFunction(nullptr), _exception(nullptr), _done(false)
		{
		}

		~WorkItem()
		{
		}

		void AttachMainFunction(WorkItemFunctionPtr func) override;

		void AttachPosteriorFunction(WorkItemFunctionPtr func) override;

		virtual Types::Result_t Schedule(ISchedulerPtr scheduler) override;

		virtual std::string GetStateString() override;

		const uint64_t GetId() override;

		std::exception_ptr GetException() const;

		virtual std::thread::id GetThreadId() override;

	};

	using WorkItemPtr = std::shared_ptr<WorkItem>;

}


#endif

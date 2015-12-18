// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/WorkItem.hpp"

// inline Logging::Loggers::Logger& logger()
// {
// 	static Logging::Loggers::Logger _logger("FuturesFramework.WorkItem");
// 	return _logger;
// }

namespace FuturesFramework
{

	void WorkItem::SetId(uint64_t id)
	{
		this->_id = id;
	}

	void WorkItem::SetThreadId(std::thread::id threadId)
	{
		this->_threadId = threadId;
	}

	void WorkItem::SetException(const std::exception_ptr pException)
	{
		this->_exception = pException;
	}

	Types::Result_t WorkItem::GetExecutionResult()
	{
		return this->_executionResult;
	}

	void WorkItem::Finish()
	{
		this->_done = true;
		this->_mainFunction = nullptr;
		this->_posteriorFunction = nullptr;
	}

	bool WorkItem::IsDone()
	{
		return this->_done;
	}

	States::WorkItemState WorkItem::GetInternalState()
	{
		return this->GetCurrentState();
	}

	void WorkItem::AttachMainFunction(WorkItemFunctionPtr func)
	{
		this->_mainFunction = func;
	}

	void WorkItem::AttachPosteriorFunction(WorkItemFunctionPtr func)
	{
		this->_posteriorFunction = func;
	}

	std::string WorkItem::GetStateString()
	{
		return GetWorkItemStateString(this->GetCurrentState());
	}

	const uint64_t WorkItem::GetId()
	{
		return this->_id;
	}

	std::thread::id WorkItem::GetThreadId()
	{
		return this->_threadId;
	}

	Types::Result_t WorkItem::Execute()
	{
		this->Trigger(Types::Result_t::SUCCESS);

		Types::Result_t result = Types::Result_t::FAILURE;
		if (this->_mainFunction)
		{
			try
			{
				result = this->_mainFunction();
			}
			catch (...)
			{
				this->_exception = std::current_exception();
			}
		}
		else
		{
			throw std::logic_error("No function to execute!");
		}
		this->Trigger(result);

		if (result != Types::Result_t::FAILURE && this->_posteriorFunction)
		{
			try
			{
				this->_posteriorFunction();
			}
			catch (...)
			{
				result = Types::Result_t::FAILURE;
			}
		}
		this->Trigger(result);

		this->_executionResult = result;

		return result;
	}

	std::exception_ptr WorkItem::GetException() const
	{
		return this->_exception;
	}

	Types::Result_t WorkItem::Schedule(ISchedulerPtr scheduler)
	{
		std::cout << "Entering WorkItem::Schedule() with state: " << this->WorkItem::GetStateString() << std::endl;
		// if (scheduler->ScheduleWorkItem(std::dynamic_pointer_cast<IWorkItem>(this->shared_from_this())))
		Types::Result_t result = Types::Result_t::FAILURE;
		if(scheduler->ScheduleWorkItem(shared_from_this()))
		{
			result = Types::Result_t::SUCCESS;
		}
		std::cout << "Exiting WorkItem::Schedule() with state: " << this->WorkItem::GetStateString() << std::endl;
		return result;
	}
}

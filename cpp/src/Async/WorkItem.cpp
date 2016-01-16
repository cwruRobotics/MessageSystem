// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/WorkItem.hpp"

namespace Async
{

	void WorkItem::SetId(uint64_t id)
	{
		this->_id = id;
	}

	void WorkItem::SetException(const std::exception_ptr pException)
	{
		this->_pException = pException;
	}

	void WorkItem::Finish()
	{
        this->_pMainFunction = nullptr;
		this->_pPostFunction = nullptr;
	}

	bool WorkItem::IsDone()
	{
		return this->GetCurrentState() == States::WorkItemState::DONE;
	}

	void WorkItem::AttachMainFunction(FunctionPtr func)
	{
		this->_pMainFunction = func;
	}

	void WorkItem::AttachPosteriorFunction(FunctionPtr func)
	{
		this->_pPostFunction = func;
	}

	const uint64_t WorkItem::GetId()
	{
		return this->_id;
	}

	Types::Result_t WorkItem::Execute()
	{
		this->Trigger(Types::Result_t::SUCCESS);

		Types::Result_t result = Types::Result_t::FAILURE;
		if (this->_pMainFunction)
		{
			try
			{
				result = this->_pMainFunction();
                this->Trigger(result);
			}
			catch (...)
			{
                this->Trigger(Types::Result_t::SUCCESS);
				this->SetException(std::current_exception());
			}
		}
		else
		{
			throw std::logic_error("No function to execute!");
		}
		this->Trigger(result);

		if (result != Types::Result_t::FAILURE && this->_pPostFunction)
		{
			try
			{
				this->_pPostFunction();
			}
			catch (...)
			{
				result = Types::Result_t::FAILURE;
			}
		}
		this->Trigger(result);

		return result;
	}

	std::exception_ptr WorkItem::GetException() const
	{
		return this->_pException;
	}

	Types::Result_t WorkItem::Schedule(ISchedulerPtr scheduler)
	{
		// std::cout << "Entering WorkItem::Schedule() with state: " << this->WorkItem::GetStateString() << std::endl;
		// if (scheduler->ScheduleWorkItem(std::dynamic_pointer_cast<IWorkItem>(this->shared_from_this())))
		Types::Result_t result = Types::Result_t::FAILURE;
		if(scheduler->ScheduleWorkItem(shared_from_this()))
		{
			result = Types::Result_t::SUCCESS;
		}
		// std::cout << "Exiting WorkItem::Schedule() with state: " << this->WorkItem::GetStateString() << std::endl;
		return result;
	}

    const std::string WorkItem::GetStateAsString()
    {
        return GetWorkItemStateString(this->GetCurrentState());
    }

    const Types::JobPriority WorkItem::GetPriority()
    {
        return this->_jobPriority;
    }
}

// SYSTEM INCLUDES

// C++ PROJECT INCLUDES
#include "Async/WorkItem.hpp"

namespace Async
{

    FunctionPtr WorkItem::GetPosteriorFunction()
    {
        if(this->_pPostFunction)
        {
            return this->_pPostFunction;
        }
        return []() -> States::WorkItemState
        {
            return States::WorkItemState::DONE;
        };
    }

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

    States::WorkItemState WorkItem::GetState()
    {
        return this->_innerState;
    }

    void WorkItem::SetState(States::WorkItemState newState)
    {
        this->_innerState = newState;
    }

	bool WorkItem::IsDone()
	{
		return this->_innerState == States::WorkItemState::DONE;
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

    States::WorkItemState WorkItem::Execute()
    {
        States::WorkItemState nextState = States::WorkItemState::DONE;
        if (this->_pMainFunction)
        {
            try
            {
                nextState = this->_pMainFunction();
            }
            catch (...)
            {
                this->SetException(std::current_exception());
            }
        }
        else
        {
            throw std::logic_error("No function to execute!");
        }

        FunctionPtr pPost = this->GetPosteriorFunction();
        if (nextState != States::WorkItemState::DONE && pPost)
        {
            try
            {
                nextState = pPost();
            }
            catch (...)
            {
                this->SetException(std::current_exception());
            }
        }

        this->_innerState = nextState;
        return nextState;
	}

	std::exception_ptr WorkItem::GetException() const
	{
		return this->_pException;
	}

	Types::Result_t WorkItem::Schedule(SchedulerBasePtr scheduler)
	{

		Types::Result_t result = Types::Result_t::FAILURE;

        if(!scheduler || (this->_innerState != States::WorkItemState::IDLE &&
           this->_innerState != States::WorkItemState::RESCHEDULE))
        {
            return result;
        }

        this->_innerState = States::WorkItemState::SCHEDULE;
		if(scheduler->ScheduleWorkItem(shared_from_this()))
		{
			result = Types::Result_t::SUCCESS;
		}
		// std::cout << "Exiting WorkItem::Schedule() with state: " << this->WorkItem::GetStateString() << std::endl;
		return result;
	}

    const std::string WorkItem::GetStateAsString()
    {
        return GetWorkItemStateString(this->_innerState);
    }

    const Types::JobPriority WorkItem::GetPriority()
    {
        return this->_jobPriority;
    }

}

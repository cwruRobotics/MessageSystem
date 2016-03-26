// SYSTEM INCLUDES
#include <iostream>
#include <stdexcept>

// C++ PROJECT INCLUDES
//#include "Async/ContinuableWorkItem.hpp"
#include "Async/WorkItem.hpp"
#include "Async/PromiseBase.hpp"

namespace Async
{

    PromiseBaseImpl::PromiseBaseImpl(Types::JobPriority priority) :
        _state(States::SettlementState::PENDING), _successSuccessors(),
        _failureSuccessors(), _pWorkItem(std::make_shared<WorkItem>(0, priority))
    {
    }

    PromiseBaseImpl::~PromiseBaseImpl()
    {
    }

    bool PromiseBaseImpl::IsDone()
    {
        return std::dynamic_pointer_cast<WorkItem>(this->_pWorkItem)->IsDone();
    }

    void PromiseBaseImpl::AddContinuation(ChainLinkerBasePtr ptr,
        bool onSuccess)
	{
        if (onSuccess)
        {
            this->_successSuccessors.push_back(ptr);
        }
        else
        {
            this->_failureSuccessors.push_back(ptr);
        }
	}

    void PromiseBaseImpl::AttachMainFunction(FunctionPtr pFunc)
    {
        std::dynamic_pointer_cast<WorkItem>(this->_pWorkItem)->AttachMainFunction(pFunc);
    }

    void PromiseBaseImpl::AttachPosteriorFunction(FunctionPtr pFunc)
    {
        std::dynamic_pointer_cast<WorkItem>(this->_pWorkItem)->AttachPosteriorFunction(pFunc);
    }

    States::WorkItemState PromiseBaseImpl::Execute()
	{
		States::WorkItemState nextState =
            std::dynamic_pointer_cast<WorkItem>(this->_pWorkItem)->Execute();

		if (nextState == States::WorkItemState::DONE)
		{
            std::vector<ChainLinkerBasePtr>& successorVector = this->_successSuccessors;
            if (this->GetException())
            {
                this->SetFailure();

                successorVector = this->_failureSuccessors;
            }

			for (ChainLinkerBasePtr& successor : successorVector)
			{
				successor->Chain();
			}
		}
		// this->SetExternalState(States::PromiseState::UNTOUCHED);
		// this logic is called!!! If we set the External State to States::PromiseStates::UNTOUCHED
		// it will return the value we set as well as 
		return nextState;
	}

    void PromiseBaseImpl::SetSuccess()
	{
		this->_state = States::SettlementState::SUCCESS;
	}

    void PromiseBaseImpl::SetFailure()
	{
		this->_state = States::SettlementState::FAILURE;
	}

    const States::SettlementState PromiseBaseImpl::GetState()
    {
        return this->_state;
    }

    bool PromiseBaseImpl::PreconditionsMet()
    {
        return true;
    }

    Types::Result_t PromiseBaseImpl::Schedule(SchedulerBasePtr scheduler)
	{
		Types::Result_t result = Types::Result_t::FAILURE;

        WorkItemPtr pWorkItem = std::dynamic_pointer_cast<WorkItem>(this->_pWorkItem);

        if(!scheduler || (pWorkItem->GetState() != States::WorkItemState::IDLE &&
           pWorkItem->GetState() != States::WorkItemState::RESCHEDULE))
        {
            return result;
        }

        pWorkItem->SetState(States::WorkItemState::SCHEDULE);
		if(scheduler->ScheduleWorkItem(shared_from_this()))
		{
			result = Types::Result_t::SUCCESS;
		}
		// std::cout << "Exiting WorkItem::Schedule() with state: " << this->WorkItem::GetStateString() << std::endl;
		return result;
	}

    void PromiseBaseImpl::SetId(uint64_t id)
    {
        this->_pWorkItem->SetId(id);
    }

    const uint64_t PromiseBaseImpl::GetId()
	{
		return this->_pWorkItem->GetId();
	}

    std::exception_ptr PromiseBaseImpl::GetException() const
    {
        return this->_pWorkItem->GetException();
    }

    const Types::JobPriority PromiseBaseImpl::GetPriority()
    {
        return std::dynamic_pointer_cast<WorkItem>(this->_pWorkItem)->GetPriority();
    }

}

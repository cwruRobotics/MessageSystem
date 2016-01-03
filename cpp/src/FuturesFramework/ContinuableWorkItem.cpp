// SYSTEM INCLUDES



// C++ PROJECT INCLUDES
#include "FuturesFramework/ContinuableWorkItem.hpp"

namespace FuturesFramework
{

    void ContinuableWorkItem::SetState(States::SettlementState newState)
    {
        this->_state = newState;
    }

	void ContinuableWorkItem::SetSuccess()
	{
		this->_state = States::SettlementState::SUCCESS;
	}

	void ContinuableWorkItem::SetFailure()
	{
		this->_state = States::SettlementState::FAILURE;
	}

    const States::SettlementState ContinuableWorkItem::GetState()
    {
        return this->_state;
    }

	bool ContinuableWorkItem::IsCurrentlyExecuting()
	{
        States::WorkItemState state = this->GetCurrentState();
		return (state == States::WorkItemState::EVALUATING_PRECONDITIONS ||
            state == States::WorkItemState::EXECUTING_MAIN_FUNCTION ||
			state == States::WorkItemState::EXECUTING_POSTERIOR_FUNCTION);
	}

	Types::Result_t ContinuableWorkItem::Execute()
	{
		Types::Result_t result = this->WorkItem::Execute();
		if (result == Types::Result_t::FAILURE)
		{
			// do OnError callback here if it exists.
            for (IChainLinkerPtr& successor : this->_failureSuccessors)
            {
                successor->Chain();
            }
		}
		else if (result == Types::Result_t::SUCCESS)
		{
			// do Then callback here if it exists.
			for (IChainLinkerPtr& successor : this->_successSuccessors)
			{
				successor->Chain();
			}
		}

		// this->SetExternalState(States::PromiseState::UNTOUCHED);
		// this logic is called!!! If we set the External State to States::PromiseStates::UNTOUCHED
		// it will return the value we set as well as 
		return result;
	}

	void ContinuableWorkItem::AddContinuation(IChainLinkerPtr ptr,
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

}

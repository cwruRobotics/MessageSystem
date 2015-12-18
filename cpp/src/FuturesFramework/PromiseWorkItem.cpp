// SYSTEM INCLUDES



// C++ PROJECT INCLUDES
#include "FuturesFramework/PromiseWorkItem.hpp"
#include "FuturesFramework/PromiseStates.hpp"

namespace FuturesFramework
{

	void PromiseWorkItem::SetSuccess()
	{
		this->SetExternalState(States::PromiseState::SUCCESS);
	}

	void PromiseWorkItem::SetFailure()
	{
		this->SetExternalState(States::PromiseState::FAILURE);
	}

	States::WorkItemState PromiseWorkItem::GetWorkItemState()
	{
		return this->GetInternalState();
	}

	void PromiseWorkItem::SetExternalState(States::PromiseState newState)
	{
		this->_externalState = newState;
	}

	States::PromiseState PromiseWorkItem::GetState()
	{
		return this->_externalState;
	}

	std::string PromiseWorkItem::GetStateString()
	{
		return GetPromiseStateString(this->_externalState);
	}

	std::string PromiseWorkItem::GetInternalStateString()
	{
		return this->WorkItem::GetStateString();
	}

	bool PromiseWorkItem::IsCurrentlyExecuting()
	{
		return (this->GetInternalState() == States::WorkItemState::EXECUTING_MAIN_FUNCTION ||
			this->GetInternalState() == States::WorkItemState::EXECUTING_POSTERIOR_FUNCTION) &&
			(this->GetThreadId() == std::this_thread::get_id());
	}

	Types::Result_t PromiseWorkItem::Execute()
	{
		Types::Result_t result = this->WorkItem::Execute();
		if (result == Types::Result_t::FAILURE)
		{
			// do OnError callback here if it exists.
		}
		else if (result == Types::Result_t::SUCCESS)
		{
			// do Then callback here if it exists.
			for (IChainLinkerPtr& successor : this->_chainLinker)
			{
				successor->Chain();
			}
		}

		// this->SetExternalState(States::PromiseState::UNTOUCHED);
		// this logic is called!!! If we set the External State to States::PromiseStates::UNTOUCHED
		// it will return the value we set as well as 
		return result;
	}

	void PromiseWorkItem::AddContinuation(IChainLinkerPtr ptr)
	{
		this->_chainLinker.push_back(ptr);
	}

}

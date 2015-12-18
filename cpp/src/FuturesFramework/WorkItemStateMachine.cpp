// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/WorkItemStateMachine.hpp"

namespace FuturesFramework
{

	void WorkItemStateMachine::SetState(States::WorkItemState newState)
	{
		this->_currentState = newState;
	}

	States::WorkItemState WorkItemStateMachine::GetCurrentState()
	{
		return this->_currentState;
	}

	States::WorkItemState WorkItemStateMachine::TriggerIdle(Types::Result_t trigger)
	{
		States::WorkItemState newState = States::WorkItemState::IDLE;
		if (trigger == Types::Result_t::SUCCESS)
		{
			newState = States::WorkItemState::SCHEDULE;
		}
		else if(trigger != Types::Result_t::FAILURE)
		{
			throw std::logic_error("Weird State from Idle: " + ResultToString(trigger));
		}

		return newState;
	}

	States::WorkItemState WorkItemStateMachine::TriggerExecutingMain(Types::Result_t trigger)
	{
		States::WorkItemState newState = States::WorkItemState::DONE;
		if (trigger == Types::Result_t::SUCCESS)
		{
			newState = States::WorkItemState::EXECUTING_POSTERIOR_FUNCTION;
		}
		else if(trigger != Types::Result_t::FAILURE)
		{
			throw std::logic_error("Weird State from Executing Main: " + ResultToString(trigger));
		}
		return newState;
	}

	States::WorkItemState WorkItemStateMachine::TriggerExecutingPosterior(Types::Result_t trigger)
	{
		if(trigger != Types::Result_t::FAILURE && trigger != Types::Result_t::SUCCESS)
		{
			throw std::logic_error("Weird State from Executing Posterior: " + ResultToString(trigger));
		}
		return States::WorkItemState::DONE;
	}

	States::WorkItemState WorkItemStateMachine::TriggerSchedule(Types::Result_t trigger)
	{
		States::WorkItemState newState = States::WorkItemState::DONE;
		if (trigger == Types::Result_t::SUCCESS)
		{
			newState = States::WorkItemState::EXECUTING_MAIN_FUNCTION;
		}
		else if(trigger != Types::Result_t::FAILURE)
		{
			throw std::logic_error("Weird State from Schedule: " + ResultToString(trigger));
		}
		return newState;
	}

	States::WorkItemState WorkItemStateMachine::TriggerReSchedule(Types::Result_t trigger)
	{
		States::WorkItemState newState = States::WorkItemState::DONE;
		if (trigger == Types::Result_t::SUCCESS)
		{
			newState = States::WorkItemState::EXECUTING_MAIN_FUNCTION;
		}
		else if (trigger != Types::Result_t::FAILURE)
		{
			throw std::logic_error("Weird State from Schedule: " + ResultToString(trigger));
		}
		return newState;
	}

	States::WorkItemState WorkItemStateMachine::TriggerDone(Types::Result_t trigger)
	{
		return States::WorkItemState::DONE;
	}

	States::WorkItemState WorkItemStateMachine::Trigger(Types::Result_t trigger)
	{
		States::WorkItemState newState = States::WorkItemState::DONE;
		switch (this->GetCurrentState())
		{
		case States::WorkItemState::IDLE:
			newState = this->TriggerIdle(trigger);
			break;

		case States::WorkItemState::EXECUTING_MAIN_FUNCTION:
			newState = this->TriggerExecutingMain(trigger);
			break;

		case States::WorkItemState::EXECUTING_POSTERIOR_FUNCTION:
			newState = this->TriggerExecutingPosterior(trigger);
			break;

		case States::WorkItemState::SCHEDULE:
			newState = this->TriggerSchedule(trigger);
			break;

		case States::WorkItemState::RESCHEDULE:
			newState = this->TriggerReSchedule(trigger);
			break;

		case States::WorkItemState::DONE:
			newState = this->TriggerDone(trigger);
			break;
		}
		this->SetState(newState);
		return this->GetCurrentState();
	}

	States::WorkItemState WorkItemStateMachine::Cancel()
	{
		States::WorkItemState newState = States::WorkItemState::DONE;
		if (this->GetCurrentState() != States::WorkItemState::EXECUTING_MAIN_FUNCTION &&
			this->GetCurrentState() != States::WorkItemState::EXECUTING_POSTERIOR_FUNCTION
			&& this->GetCurrentState() != States::WorkItemState::DONE)
		{
			this->SetState(newState);
		}
		return this->GetCurrentState();
	}

}

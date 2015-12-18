#pragma once

#ifndef FUTURESFRAMEWORK_WORKITEMSTATEMACHINE_HPP
#define FUTURESFRAMEWORK_WORKITEMSTATEMACHINE_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/IWorkItemStateMachine.hpp"

namespace FuturesFramework
{

	class WorkItemStateMachine : public IWorkItemStateMachine
	{
	private:

		States::WorkItemState	_currentState;

		void SetState(States::WorkItemState newState);

	protected:

		States::WorkItemState TriggerIdle(Types::Result_t trigger);

		States::WorkItemState TriggerExecutingMain(Types::Result_t trigger);

		States::WorkItemState TriggerExecutingPosterior(Types::Result_t trigger);

		States::WorkItemState TriggerSchedule(Types::Result_t trigger);

		States::WorkItemState TriggerReSchedule(Types::Result_t trigger);

		States::WorkItemState TriggerDone(Types::Result_t trigger);

		virtual States::WorkItemState Trigger(Types::Result_t trigger) override;

		virtual States::WorkItemState Cancel();

	public:

		WorkItemStateMachine() : _currentState(States::WorkItemState::IDLE)
		{
		}


		States::WorkItemState GetCurrentState() override;

	};

}


#endif // !FUTURESFRAMEWORK_WORKITEMSTATEMACHINE_HPP

#pragma once

#ifndef FUTURESFRAMEWORK_WORKITEMSTATEMACHINE_HPP
#define FUTURESFRAMEWORK_WORKITEMSTATEMACHINE_HPP

// SYSTEM INCLUDES
#include <map>
#include <vector>

// C++ PROJECT INCLUDES
#include "FuturesFramework/Result.hpp"
#include "FuturesFramework/WorkItemStates.hpp"

namespace FuturesFramework
{

    class WorkItemStateMachine
    {
    private:

        States::WorkItemState  _currentState;

        void SetState(States::WorkItemState newState);

    protected:

        States::WorkItemState TriggerIdle(Types::Result_t trigger);

        States::WorkItemState TriggerEvaluatingPreConditions(Types::Result_t trigger);

		States::WorkItemState TriggerExecutingMain(Types::Result_t trigger);

		States::WorkItemState TriggerExecutingPosterior(Types::Result_t trigger);

		States::WorkItemState TriggerSchedule(Types::Result_t trigger);

		States::WorkItemState TriggerReSchedule(Types::Result_t trigger);

		States::WorkItemState TriggerDone(Types::Result_t trigger);

		virtual States::WorkItemState Trigger(Types::Result_t trigger);

		virtual States::WorkItemState Cancel();

        States::WorkItemState GetCurrentState();

    public:

        WorkItemStateMachine(States::WorkItemState initState) :
            _currentState(initState)
        {
        }

    };

}

#endif

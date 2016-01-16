#pragma once

#ifndef ASYNC_WORKITEMSTATEMACHINE_HPP
#define ASYNC_WORKITEMSTATEMACHINE_HPP

// SYSTEM INCLUDES
#include <map>
#include <vector>

// C++ PROJECT INCLUDES
#include "Async/Result.hpp"
#include "Async/WorkItemStates.hpp"

// project namespace
namespace Async
{

    // the state machine that all built in WorkItems implement.
    // in the future, a WorkItem will CONTAIN an IWorkItemStateMachine,
    // rather than derive from it. This will allow clients to have
    // their own asynchronous data structures be able to exercise
    // client written state machines.
    class WorkItemStateMachine
    {
    private:

        States::WorkItemState  _currentState;

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

        void SetState(States::WorkItemState newState);

    public:

        WorkItemStateMachine(States::WorkItemState initState) :
            _currentState(initState)
        {
        }

    };

} // end of namespace Async

#endif // end of header guard

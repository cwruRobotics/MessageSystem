
#pragma once

#ifndef ASYNC_UNITTEST_WORKITEMSTATEMACHINEUNITTESTINTERFACE_HPP
#define ASYNC_UNITTEST_WORKITEMSTATEMACHINEUNITTESTINTERFACE_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/Result.hpp"
#include "Async/WorkItemStates.hpp"
#include "Async/WorkItemStateMachine.hpp"

namespace Async
{
namespace Tests
{

    // WorkItemStateMachine has protected methods. To test them,
    // WorkItemStateMachineUnitTestInterface will just provide public
    // methods to interface with the protected methods. This provides
    // no additional functionality, it doesn't even classify
    // as a "mock" object.
    class WorkItemStateMachineUnitTestInterface :
        public WorkItemStateMachine
    {
    public:
        WorkItemStateMachineUnitTestInterface(
            States::WorkItemState initState) :
            WorkItemStateMachine(initState)
        {
        }

        ~WorkItemStateMachineUnitTestInterface()
        {
        }

        States::WorkItemState GetCurrentState();

        States::WorkItemState Trigger(Types::Result_t trigger);

        void SetState(States::WorkItemState newState);

        States::WorkItemState Cancel();
    };

} // end of namespace Tests
} // end of namespace Async

#endif // end of header guard

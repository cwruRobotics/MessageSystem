
#pragma once

#ifndef FUTURESFRAMEWORK_UNITTEST_WORKITEMSTATEMACHINEUNITTESTINTERFACE_HPP
#define FUTURESFRAMEWORK_UNITTEST_WORKITEMSTATEMACHINEUNITTESTINTERFACE_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/Result.hpp"
#include "FuturesFramework/WorkItemStates.hpp"
#include "FuturesFramework/WorkItemStateMachine.hpp"

namespace FuturesFramework
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
} // end of namespace FuturesFramework

#endif // end of header guard

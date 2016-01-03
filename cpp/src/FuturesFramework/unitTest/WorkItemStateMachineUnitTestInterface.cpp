
// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/unitTest/WorkItemStateMachineUnitTestInterface.hpp"

namespace FuturesFramework
{
namespace Tests
{

    States::WorkItemState WorkItemStateMachineUnitTestInterface::
        GetCurrentState()
    {
        return this->WorkItemStateMachine::GetCurrentState();
    }

    States::WorkItemState WorkItemStateMachineUnitTestInterface::
        Trigger(Types::Result_t trigger)
    {
        return this->WorkItemStateMachine::Trigger(trigger);
    }

    void WorkItemStateMachineUnitTestInterface::SetState(
        States::WorkItemState newState)
    {
        this->WorkItemStateMachine::SetState(newState);
    }

    States::WorkItemState WorkItemStateMachineUnitTestInterface::
        Cancel()
    {
        return this->WorkItemStateMachine::Cancel();
    }

} // end of namespace Tests
} // end of namespace FuturesFramework

#pragma once

#ifndef ASYNC_STATES_WORKITEMSTATES
#define ASYNC_STATES_WORKITEMSTATES

// SYSTEM INCLUDES
#include <stdexcept>
#include <string>

// C++ PROJECT INCLUDES
// (none)

// project namespace
namespace Async
{
// component namespace
namespace States
{

    // the states that a WorkItem exercises.
    // These influence how a WorkItem behaves in its
    // execution.
	enum class WorkItemState
	{
		IDLE = 0,                           // nothing has happend to a WorkItem yet.
        EVALUATING_PRECONDITIONS = 1,       // WorkItem is evaluating preconditions.
		EXECUTING_MAIN_FUNCTION = 2,        // WorkItem is executing its main function.
		EXECUTING_POSTERIOR_FUNCTION = 3,   // WorkItem is executing "cleanup" function.
		SCHEDULE = 4,                       // WorkItem has been scheduled for execution.
		RESCHEDULE = 5,                     // WorkItem has been rescheduled.
		DONE = 6,                           // WorkItem execution is done.
	};

} // end of namespace States

// Get the string representation of a States::WorkItemState
std::string GetWorkItemStateString(States::WorkItemState state);

} // end of namespace Async

#endif // end of header guard

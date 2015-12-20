#pragma once

#ifndef FUTURESFRAMEWORK_STATES_WORKITEMSTATES
#define FUTURESFRAMEWORK_STATES_WORKITEMSTATES

// SYSTEM INCLUDES
#include <stdexcept>
#include <string>

// C++ PROJECT INCLUDES
// (none)

namespace FuturesFramework
{
namespace States
{

	enum class WorkItemState
	{
		IDLE = 0,
        EVALUATING_PRECONDITIONS = 1,
		EXECUTING_MAIN_FUNCTION = 2,
		EXECUTING_POSTERIOR_FUNCTION = 3,
		SCHEDULE = 4,
		RESCHEDULE = 5,
		DONE = 6,
	};

} // end of namespace States

std::string GetWorkItemStateString(States::WorkItemState state);

} // end of namespace FuturesFramework


#endif

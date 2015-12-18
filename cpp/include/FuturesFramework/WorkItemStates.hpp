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
		EXECUTING_MAIN_FUNCTION = 1,
		EXECUTING_POSTERIOR_FUNCTION = 2,
		SCHEDULE = 3,
		RESCHEDULE = 4,
		DONE = 5,
	};

} // end of namespace States

std::string GetWorkItemStateString(States::WorkItemState state);

} // end of namespace FuturesFramework


#endif
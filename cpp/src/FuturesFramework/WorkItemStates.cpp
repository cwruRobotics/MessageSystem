// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/WorkItemStates.hpp"

namespace FuturesFramework
{

	std::string GetWorkItemStateString(States::WorkItemState state)
	{
		switch (state)
		{
		case States::WorkItemState::DONE:
			return "Done";
		case States::WorkItemState::IDLE:
			return "Idle";
		case States::WorkItemState::EXECUTING_MAIN_FUNCTION:
			return "Executing Main Function";
		case States::WorkItemState::EXECUTING_POSTERIOR_FUNCTION:
			return "Executing Posterior Function";
		case States::WorkItemState::RESCHEDULE:
			return "Reschedule";
		case States::WorkItemState::SCHEDULE:
			return "Schedule";
		}
		throw std::logic_error("Unexpected WorkItem State");
	}

}

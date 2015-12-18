#pragma once

#ifndef FUTURESFRAMEWORK_STATES_IWORKITEMSTATEMACHINE_HPP
#define FUTURESFRAMEWORK_STATES_IWORKITEMSTATEMACHINE_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/Result.hpp"
#include "FuturesFramework/WorkItemStates.hpp"

namespace FuturesFramework
{
	class IWorkItemStateMachine
	{
	protected:

		virtual States::WorkItemState Trigger(Types::Result_t trigger) = 0;

	public:

		virtual ~IWorkItemStateMachine()
		{
		}

		virtual States::WorkItemState GetCurrentState() = 0;

	};
}

#endif

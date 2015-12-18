#pragma once

#ifndef FUTURESFRAMEWORK_IPROMISEWORKITEM_HPP
#define FUTURESFRAMEWORK_IPROMISEWORKITEM_HPP

// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES
#include "FuturesFramework/IChainLinker.hpp"
#include "FuturesFramework/PromiseStates.hpp"
#include "FuturesFramework/WorkItemStates.hpp"

namespace FuturesFramework
{

	class IPromiseWorkItem;
	using IPromiseWorkItemPtr = std::shared_ptr<IPromiseWorkItem>;

	class IPromiseWorkItem
	{
	public:

		virtual ~IPromiseWorkItem() = default;

		virtual void SetSuccess() = 0;

		virtual void SetFailure() = 0;

		virtual States::PromiseState GetState() = 0;

		virtual std::string GetInternalStateString() = 0;

		virtual bool IsCurrentlyExecuting() = 0;

		virtual void AddContinuation(IChainLinkerPtr continuation) = 0;

	};

}


#endif

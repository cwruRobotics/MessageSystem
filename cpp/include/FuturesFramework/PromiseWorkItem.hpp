#pragma once

#ifndef FUTURESFRAMEWORK_PROMISEWORKITEM_HPP
#define FUTURESFRAMEWORK_PROMISEWORKITEM_HPP

// SYSTEM INCLUDES
#include <vector>

// C++ PROJECT INCLUDES
#include "FuturesFramework/IPromiseWorkItem.hpp"
#include "FuturesFramework/WorkItem.hpp"

namespace FuturesFramework
{

	class PromiseWorkItem;
	using PromiseWorkItemPtr = std::shared_ptr<PromiseWorkItem>;

	class PromiseWorkItem : public virtual IPromiseWorkItem, public WorkItem
	{
	private:

		States::PromiseState			_externalState;
		std::vector<IChainLinkerPtr>	_chainLinker;

		void SetExternalState(States::PromiseState newState);

		States::WorkItemState GetWorkItemState();

	protected:

		Types::Result_t Execute() override;

	public:

		PromiseWorkItem(): WorkItem(), _externalState(States::PromiseState::UNTOUCHED),
			_chainLinker()
		{
		}

		~PromiseWorkItem()
		{
		}

		void SetSuccess() override;

		void SetFailure() override;

		States::PromiseState GetState() override;

		std::string GetStateString() override;

		std::string GetInternalStateString() override;

		bool IsCurrentlyExecuting() override;

		void AddContinuation(IChainLinkerPtr chainLinker) override;

	};

}



#endif

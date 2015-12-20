#pragma once

#ifndef FUTURESFRAMEWORK_CONTINUABLEWORKITEM_HPP
#define FUTURESFRAMEWORK_CONTINUABLEWORKITEM_HPP

// SYSTEM INCLUDES
#include <vector>

// C++ PROJECT INCLUDES
#include "FuturesFramework/IContinuableWorkItem.hpp"
#include "FuturesFramework/SettlementStates.hpp"
#include "FuturesFramework/WorkItem.hpp"

namespace FuturesFramework
{

    class ContinuableWorkItem;
    using ContinuableWorkItemPtr = std::shared_ptr<ContinuableWorkItem>;

    class ContinuableWorkItem : public IContinuableWorkItem, public WorkItem
    {
    private:

        States::SettlementState          _state;
        std::vector<IChainLinkerPtr>    _successors;

    private:

        void SetState(States::SettlementState newState);

    protected:

        Types::Result_t Execute() override;

    public:

        ContinuableWorkItem() : WorkItem(), _successors(),
            _state(States::SettlementState::PENDING)
        {
        }

        ~ContinuableWorkItem()
        {
        }

        void SetSuccess() override;

        void SetFailure() override;

        const States::SettlementState GetState();

        bool IsCurrentlyExecuting() override;

        void AddContinuation(IChainLinkerPtr pContinuation) override;

    };

}

#endif

#pragma once

#ifndef FUTURESFRAMEWORK_CONTINUABLEWORKITEM_HPP
#define FUTURESFRAMEWORK_CONTINUABLEWORKITEM_HPP

// SYSTEM INCLUDES
#include <vector>

// C++ PROJECT INCLUDES
#include "FuturesFramework/IContinuableWorkItem.hpp"
#include "FuturesFramework/SettlementStates.hpp"
#include "FuturesFramework/WorkItem.hpp"

// project namespace
namespace FuturesFramework
{

    // FORWARD DECLARATIONS
    class ContinuableWorkItem;

    // alias for shared pointer to ContinuableWorkItem
    using ContinuableWorkItemPtr = std::shared_ptr<ContinuableWorkItem>;

    // this class represents a WorkItem that can create child WorkItems.
    // in other words, this class specifies the functionality for
    // WorkItems to have successors.
    class ContinuableWorkItem : public IContinuableWorkItem, public WorkItem
    {
    private:

        // how did the ContinuableWorkItem resolve? This is dependent
        // upon which successors will be executed.
        States::SettlementState         _state;
        std::vector<IChainLinkerPtr>    _successors;

    private:

        void SetState(States::SettlementState newState);

    protected:

        // ContinuableWorkItem overrides the Execute method to provide
        // the continuation functionality. When the WorkItem's Execute
        // method finishes, this version tries to schedule the appropriate
        // successors.
        Types::Result_t Execute() override;

    public:

        // default constructor
        ContinuableWorkItem() : WorkItem(), _successors(),
            _state(States::SettlementState::PENDING)
        {
        }

        // default destructor
        ~ContinuableWorkItem()
        {
        }

        // set this ContinuableWorkItem as resolving successfully
        void SetSuccess() override;

        // set this ContinuableWorkItem as resolving unsuccessfully
        void SetFailure() override;

        // get the settlement state of this ContinuableWorkItem.
        // this method will be used by the client to check if the
        // current Promise has resolved and how the Promise resolved.
        const States::SettlementState GetState();

        bool IsCurrentlyExecuting() override;

        // add a successor to this ContinuableWorkItem.
        void AddContinuation(IChainLinkerPtr pContinuation) override;

    };

} // end of namespace FuturesFramework

#endif // end of header guard

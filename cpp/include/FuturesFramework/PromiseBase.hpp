#pragma once

#ifndef FUTURESFRAMEWORK_PROMISEBASE_HPP
#define FUTURESFRAMEWORK_PROMISEBASE_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/IExecutableWorkItem.hpp"
#include "FuturesFramework/ContinuableWorkItem.hpp"
#include "FuturesFramework/LibraryExport.hpp"

// project namespace
namespace FuturesFramework
{

    // a PromiseBase is all code that a Promise contains, that is
    // not template dependent. This reduces compile time.
    class FUTURESFRAMEWORK_API PromiseBase : public IWorkItem,
        public std::enable_shared_from_this<PromiseBase>
    {
    protected:

        // this is protection. It separates the public methods
        // of the internal representation from the external methods
        // available to clients.
        ContinuableWorkItemPtr _internalWorkItem;

    protected:

        void AddSuccessor(IChainLinkerPtr successorGenerator, bool onSuccess=true);

    public:
        PromiseBase() : _internalWorkItem(std::make_shared<ContinuableWorkItem>())
        {
        }

        ~PromiseBase()
        {
        }

        const States::SettlementState GetState();

        // built in Promise code relies on this method. When a Scheduler
        // tries to execute a Promise, that Promise's preconditions MUST
        // be met, or the Promise will be rescheduled until cancelled, or
        // the preconditions are met.
        virtual bool PreconditionsMet();

        Types::Result_t Schedule(ISchedulerPtr scheduler) override;

        const uint64_t GetId() override;
        
    };

} // end of namespace FuturesFramework

#endif // end of header guard

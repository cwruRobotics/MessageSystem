#pragma once

#ifndef ASYNC_PROMISEBASE_HPP
#define ASYNC_PROMISEBASE_HPP

// SYSTEM INCLUDES
#include <mutex>

// C++ PROJECT INCLUDES
#include "Async/IExecutableWorkItem.hpp"
#include "Async/IContinuableWorkItem.hpp"
#include "Async/SettlementStates.hpp"
#include "Async/LibraryExport.hpp"

// project namespace
namespace Async
{

    // a PromiseBase is all code that a Promise contains, that is
    // not template dependent. This reduces compile time.
    class ASYNC_API PromiseBase : public IWorkItem,
        public std::enable_shared_from_this<PromiseBase>
    {
    protected:

        // this is protection. It separates the public methods
        // of the internal representation from the external methods
        // available to clients.
        IContinuableWorkItemPtr _internalWorkItem;

    protected:

        void AddSuccessor(IChainLinkerPtr successorGenerator, bool onSuccess=true);

    public:
        PromiseBase();

        ~PromiseBase();

        const States::SettlementState GetState();

        // built in Promise code relies on this method. When a Scheduler
        // tries to execute a Promise, that Promise's preconditions MUST
        // be met, or the Promise will be rescheduled until cancelled, or
        // the preconditions are met.
        virtual bool PreconditionsMet();

        Types::Result_t Schedule(ISchedulerPtr scheduler) override;

        const uint64_t GetId() override;

        std::mutex& GetExecutionMutex();

    };

} // end of namespace Async

#endif // end of header guard

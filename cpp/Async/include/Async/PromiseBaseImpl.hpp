#pragma once

#ifndef ASYNC_PROMISEBASE_HPP
#define ASYNC_PROMISEBASE_HPP

// SYSTEM INCLUDES
#include <atomic>
#include <vector>

// C++ PROJECT INCLUDES
#include "Async/ExecutableWorkItemBase.hpp"
#include "Async/ContinuableWorkItemBase.hpp"
#include "Async/JobPriorities.hpp"
#include "Async/SettlementStates.hpp"
// #include "Async/IWorkItem.hpp"
#include "Async/LibraryExport.hpp"

// project namespace
namespace Async
{

    // a PromiseBase is all code that a Promise contains, that is
    // not template dependent. This reduces compile time.
    class ASYNC_API PromiseBaseImpl : public ContinuableWorkItemBase, public ExecutableWorkItemBase,
        public std::enable_shared_from_this<PromiseBaseImpl>
    {
    private:

        std::atomic<States::SettlementState>       _state;
        std::vector<ChainLinkerBasePtr>            _successSuccessors;
        std::vector<ChainLinkerBasePtr>            _failureSuccessors;

    private:

        virtual bool IsDone() override;
        
    protected:

        // this is protection. It separates the public methods
        // of the internal representation from the external methods
        // available to clients.
        ExecutableWorkItemBasePtr                  _pWorkItem;

    protected:

        States::WorkItemState Execute() override;

        // set this ContinuableWorkItem as resolving successfully
        void SetSuccess() override;

        // set this ContinuableWorkItem as resolving unsuccessfully
        void SetFailure() override;

        // add a successor to this ContinuableWorkItem.
        void AddContinuation(ChainLinkerBasePtr pContinuation,
                             bool onSuccess=true) override;

        virtual void AttachMainFunction(FunctionPtr pFunc) override;

        virtual void AttachPosteriorFunction(FunctionPtr pFunc) override;

    public:
        PromiseBaseImpl(Types::JobPriority priority=Types::JobPriority::OTHER);

        ~PromiseBaseImpl();

        const States::SettlementState GetState();

        // built in Promise code relies on this method. When a Scheduler
        // tries to execute a Promise, that Promise's preconditions MUST
        // be met, or the Promise will be rescheduled until cancelled, or
        // the preconditions are met.
        virtual bool PreconditionsMet();

        virtual Types::Result_t Schedule(SchedulerBasePtr pScheduler) override;

        virtual void SetId(uint64_t id) override;

        virtual const uint64_t GetId() override;

        virtual std::exception_ptr GetException() const override;

        const Types::JobPriority GetPriority() override;

    };

} // end of namespace Async

#endif // end of header guard

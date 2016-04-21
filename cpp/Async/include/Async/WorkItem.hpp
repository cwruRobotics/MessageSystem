#pragma once

#ifndef ASYNC_WORKITEM_HPP
#define ASYNC_WORKITEM_HPP

// SYSTEM INCLUDES

// C++ PROJECT INCLUDES
#include "Async/ExecutableWorkItemBase.hpp"
#include "Async/JobPriorities.hpp"
#include "Async/Scheduler.hpp"
#include "Async/PromiseBaseImpl.hpp"

// project namespace
namespace Async
{

    // Executable code. This is the base implementation
    // of the Promise heirarchy (excluding interfaces).
    class WorkItem : public ExecutableWorkItemBase,
        public std::enable_shared_from_this<WorkItem>
    {
        friend class Scheduler;
        friend class PromiseBaseImpl;
    private:

        uint64_t                _id;
        SchedulerBasePtr        _pScheduler;
        FunctionPtr             _pMainFunction;
        FunctionPtr             _pPostFunction;
        std::exception_ptr      _pException;
        Types::JobPriority      _jobPriority;
        States::WorkItemState   _innerState;

    private:

        FunctionPtr GetPosteriorFunction();
 
        bool IsDone() override;

    protected:

        void AttachMainFunction(FunctionPtr pFunc) override;

        void AttachPosteriorFunction(FunctionPtr pFunc) override;

        void SetException(const std::exception_ptr pException);

        virtual States::WorkItemState Execute() override;

        void Finish();

        States::WorkItemState GetState();

        void SetState(States::WorkItemState newState);

    public:

        WorkItem(uint64_t id=0, Types::JobPriority priority=Types::JobPriority::OTHER) :
            _id(id), _pScheduler(nullptr), _pMainFunction(nullptr),
            _pPostFunction(nullptr), _pException(nullptr), _jobPriority(priority), _innerState(States::WorkItemState::IDLE)
        {
            /*
            uint64_t                _id;
        SchedulerBasePtr        _pScheduler;
        FunctionPtr             _pMainFunction;
        FunctionPtr             _pPostFunction;
        std::exception_ptr      _pException;
        Types::JobPriority      _jobPriority;
        States::WorkItemState   _innerState;
            */
        }

        ~WorkItem()
        {
        }

        virtual Types::Result_t Schedule(SchedulerBasePtr pScheduler) override;

        void SetId(uint64_t id) override;

        const uint64_t GetId() override;

        virtual std::exception_ptr GetException() const override;

        const std::string GetStateAsString();

        const Types::JobPriority GetPriority() override;

    };

    // alias for shared pointer to a WorkItem instance.
    using WorkItemPtr = std::shared_ptr<WorkItem>;

} // end of namespace Async

#endif // end of header guard

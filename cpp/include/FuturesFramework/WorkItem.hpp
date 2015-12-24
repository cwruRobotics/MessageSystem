#pragma once

#ifndef FUTURESFRAMEWORK_WORKITEM_HPP
#define FUTURESFRAMEWORK_WORKITEM_HPP

// SYSTEM INCLUDES
#include <stdexcept>

// C++ PROJECT INCLUDES
#include "FuturesFramework/IExecutableWorkItem.hpp"
#include "FuturesFramework/Scheduler.hpp"
#include "FuturesFramework/WorkItemStateMachine.hpp"

namespace FuturesFramework
{

    class WorkItem : public IExecutableWorkItem, public WorkItemStateMachine,
        public std::enable_shared_from_this<WorkItem>
    {
        friend class Scheduler;
    private:

        uint64_t            _id;
        ISchedulerPtr       _pScheduler;
        FunctionPtr _pMainFunction;
        FunctionPtr _pPostFunction;
        std::exception_ptr  _pException;

    private:

        void SetId(uint64_t id);
 
        bool IsDone();

    protected:

        void SetException(const std::exception_ptr pException);

        virtual Types::Result_t Execute();

        void Finish();

    public:

        WorkItem(uint64_t id=0) : WorkItemStateMachine(States::WorkItemState::IDLE),
            _id(id), _pScheduler(nullptr), _pMainFunction(nullptr),
            _pPostFunction(nullptr), _pException(nullptr)
        {
        }

        ~WorkItem()
        {
        }

        void AttachMainFunction(FunctionPtr pFunc) override;

        void AttachPosteriorFunction(FunctionPtr pFunc) override;

        virtual Types::Result_t Schedule(ISchedulerPtr pScheduler) override;

        const uint64_t GetId() override;

        std::exception_ptr GetException() const;

    };

    using WorkItemPtr = std::shared_ptr<WorkItem>;

}


#endif

#pragma once

#ifndef FUTURESFRAMEWORK_PROMISEBASE_HPP
#define FUTURESFRAMEWORK_PROMISEBASE_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/IWorkItem.hpp"
#include "FuturesFramework/ContinuableWorkItem.hpp"
#include "FuturesFramework/LibraryExport.hpp"

namespace FuturesFramework
{

    class FUTURESFRAMEWORK_API PromiseBase : public IWorkItem,
        public std::enable_shared_from_this<PromiseBase>
    {
    protected:
        ContinuableWorkItemPtr _internalWorkItem;

    protected:
        void AddSuccessor(IChainLinkerPtr successorGenerator);

    public:
        PromiseBase() : _internalWorkItem(std::make_shared<ContinuableWorkItem>())
        {
        }

        ~PromiseBase()
        {
        }

        const States::SettlementState GetState();

        virtual bool PreconditionsMet();

        Types::Result_t Schedule(ISchedulerPtr scheduler) override;

        const uint64_t GetId() override;
        
    };

}

#endif

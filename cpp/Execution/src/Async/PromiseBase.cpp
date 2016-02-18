// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/ContinuableWorkItem.hpp"
#include "Async/PromiseBase.hpp"

namespace Async
{

    PromiseBase::PromiseBase() : _internalWorkItem(std::make_shared<ContinuableWorkItem>())
    {
    }

    PromiseBase::~PromiseBase()
    {
    }

    void PromiseBase::AddSuccessor(IChainLinkerPtr successorGenerator,
        bool onSuccess)
    {
        this->_internalWorkItem->AddContinuation(successorGenerator, onSuccess);
    }

    const States::SettlementState PromiseBase::GetState()
    {
        return std::dynamic_pointer_cast<ContinuableWorkItem>(this->_internalWorkItem)
            ->GetState();
    }

    bool PromiseBase::PreconditionsMet()
    {
        return true;
    }

    Types::Result_t PromiseBase::Schedule(ISchedulerPtr scheduler)
    {
        if (scheduler->ScheduleWorkItem(
            std::dynamic_pointer_cast<IExecutableWorkItem>(this->_internalWorkItem)))
        {
            return Types::Result_t::SUCCESS;
        }
        return Types::Result_t::FAILURE;
    }

    const uint64_t PromiseBase::GetId()
    {
        return std::dynamic_pointer_cast<IWorkItem>(
            this->_internalWorkItem)->GetId();
    }

    std::mutex& PromiseBase::GetExecutionMutex()
    {
        return std::dynamic_pointer_cast<ContinuableWorkItem>(this->_internalWorkItem)
            ->GetExecutionMutex();
    }

}

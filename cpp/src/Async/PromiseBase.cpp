// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/PromiseBase.hpp"

namespace Async
{
    void PromiseBase::AddSuccessor(IChainLinkerPtr successorGenerator,
        bool onSuccess)
    {
        this->_internalWorkItem->AddContinuation(successorGenerator, onSuccess);
    }

    const States::SettlementState PromiseBase::GetState()
    {
        return this->_internalWorkItem->GetState();
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

}

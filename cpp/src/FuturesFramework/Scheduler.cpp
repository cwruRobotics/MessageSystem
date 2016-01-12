// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/Scheduler.hpp"
#include "FuturesFramework/WorkItem.hpp"

namespace FuturesFramework
{

	std::map<uint64_t, IExecutableWorkItemPtr>& Scheduler::GetWorkItemMap()
	{
		return this->_attachedWorkItems;
	}

	std::map<Types::JobPriority, Concurrency::IThreadPtr>& Scheduler::GetThreadMap()
	{
		return this->_threadMap;
	}

	bool Scheduler::DetachWorkItem(uint64_t id)
	{
        std::lock_guard<std::mutex> lock(this->_mutex);
		auto index = this->GetWorkItemMap().find(id);
		if (index == this->GetWorkItemMap().end())
		{
			return false;
		}
		IExecutableWorkItemPtr workItem = index->second;
		// set this WorkItem's SchedulerPtr to null;
		
		this->GetWorkItemMap().erase(index);
		return true;
	}

	bool Scheduler::ScheduleWorkItem(IExecutableWorkItemPtr workItem)
	{
        std::lock_guard<std::mutex> lock(this->_mutex);
		if (workItem)
		{
			WorkItemPtr castedWorkItem = std::dynamic_pointer_cast<WorkItem>(workItem);
			castedWorkItem->SetId(++this->_currentWorkItemId);

			this->GetWorkItemMap().insert(std::pair<uint64_t, IExecutableWorkItemPtr>(this->_currentWorkItemId, workItem));

            // this will actually allow WorkerThreads to execute WorkItems!
            // they are concurrently protected, but the condition variable
            // is not yet so be careful!
            auto index = this->GetThreadMap().find(castedWorkItem->GetPriority());
            if (index == this->GetThreadMap().end())
            {
                return false;
            }

            Types::Result_t result = index->second->Queue(workItem);
			castedWorkItem->Trigger(Types::Result_t::SUCCESS);
			return result == Types::Result_t::SUCCESS;
		}
		return false;
	}

    void Scheduler::Shutdown()
    {
        std::lock_guard<std::mutex> lock(this->_mutex);
        if (this->_running)
        {
            this->_running = false;
            for (auto it = this->_threadMap.begin(); it != this->_threadMap.end(); ++it)
            {
                it->second->Join();
            }
        }
    }

    const uint64_t Scheduler::GetCurrentWorkItemId()
    {
        return this->_currentWorkItemId;
    }

}

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

	std::map<std::thread::id, Concurrency::IThreadPtr>& Scheduler::GetThreadMap()
	{
		return this->_threadMap;
	}

    // std::queue<IExecutableWorkItemPtr>& Scheduler::GetQueue()
    // {
    //     return this->_queue;
    // }

    bool Scheduler::ExecuteWorkItem(const uint64_t id)
    {
        auto index = this->GetWorkItemMap().find(id);
		if (index == this->GetWorkItemMap().end())
		{
			return false;
		}
		WorkItemPtr toExecute = std::dynamic_pointer_cast<WorkItem>(index->second);
		Types::Result_t result = toExecute->Execute();
		return true;
    }

	bool Scheduler::DetachWorkItem(uint64_t id)
	{
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
        std::lock_guard<std::mutex>(this->_mutex);
		if (workItem)
		{
			WorkItemPtr castedWorkItem = std::dynamic_pointer_cast<WorkItem>(workItem);
			castedWorkItem->SetId(++this->_currentWorkItemId);

			this->GetWorkItemMap().insert(std::pair<uint64_t, IExecutableWorkItemPtr>(this->_currentWorkItemId, workItem));

            // this will actually allow WorkerThreads to execute WorkItems!
            // they are concurrently protected, but the condition variable
            // is not yet so be careful!
            // this->_tempWorkerThread->Queue(workItem);

			castedWorkItem->Trigger(Types::Result_t::SUCCESS);
			return true;
		}
		return false;
	}

	void Scheduler::Run()
	{
		return;
	}

    void Scheduler::Shutdown()
    {
        if (this->_running)
        {
            this->_running = false;
            // for now
            // this->_tempWorkerThread->Shutdown();

            // in the future
            // for (auto it = this->_threadMap.begin(); it != this->_threadMap.end(); ++it)
            // {
            //     it->second->Shutdown();
            // }
        }
    }

    const uint64_t Scheduler::GetCurrentWorkItemId()
    {
        return this->_currentWorkItemId;
    }

}

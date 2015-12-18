// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/Scheduler.hpp"
#include "FuturesFramework/WorkItem.hpp"

namespace FuturesFramework
{

	std::map<uint64_t, IWorkItemPtr>& Scheduler::GetWorkItemMap()
	{
		return this->_attachedWorkItems;
	}

	std::map<std::thread::id, std::thread>& Scheduler::GetThreadMap()
	{
		return this->_threadMap;
	}

	bool Scheduler::DetachWorkItem(uint64_t id)
	{
		auto index = this->GetWorkItemMap().find(id);
		if (index == this->GetWorkItemMap().end())
		{
			return false;
		}
		IWorkItemPtr workItem = index->second;
		// set this WorkItem's SchedulerPtr to null;
		
		this->GetWorkItemMap().erase(index);
		return true;
	}

	bool Scheduler::ScheduleWorkItem(IWorkItemPtr workItem)
	{
		if (workItem)
		{
			WorkItemPtr castedWorkItem = std::dynamic_pointer_cast<WorkItem>(workItem);
			castedWorkItem->SetId(++this->_currentWorkItemId);

			this->GetWorkItemMap().insert(std::pair<uint64_t, IWorkItemPtr>(this->_currentWorkItemId, workItem));

			castedWorkItem->Trigger(Types::Result_t::SUCCESS);
			// this->_executionQueue.push(workItem);
			return true;
		}
		return false;
	}

	void Scheduler::Run()
	{
		return;
	}

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

}

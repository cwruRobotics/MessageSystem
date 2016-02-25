// SYSTEM INCLUDES
#include <Logging/Factory.hpp>
#include <Logging/ILogger.hpp>

// C++ PROJECT INCLUDES
#include "Async/Scheduler.hpp"
#include "Async/WorkItem.hpp"

namespace Async
{

	std::map<Types::JobPriority, Concurrency::IThreadPtr>& Scheduler::GetThreadMap()
	{
		return this->_threadMap;
	}

	bool Scheduler::ScheduleWorkItem(IExecutableWorkItemPtr workItem)
	{
		if (workItem)
		{
			WorkItemPtr castedWorkItem = std::dynamic_pointer_cast<WorkItem>(workItem);
			castedWorkItem->SetId(++this->_currentWorkItemId);

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
        // Logging::LoggerPtr pLogger = Logging::Factory::MakeLogger("ConsoleLogger");
        // LOG_INFO(pLogger, "%s", "Logging to Console!");
        if (this->_running)
        {
            this->_running = false;
            for (auto it = this->_threadMap.begin(); it != this->_threadMap.end(); ++it)
            {
                it->second->Join();
            }
        }
    }

    bool Scheduler::IsRunning()
    {
        return this->_running;
    }

    const uint64_t Scheduler::GetCurrentWorkItemId()
    {
        return this->_currentWorkItemId;
    }

}

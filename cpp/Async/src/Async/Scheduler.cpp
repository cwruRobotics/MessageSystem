// SYSTEM INCLUDES
#include <iostream>
#include <Logging/Factory.hpp>
#include <Logging/ILogger.hpp>

// C++ PROJECT INCLUDES
#include "Async/Scheduler.hpp"
#include "Async/WorkItem.hpp"

namespace Async
{

    Scheduler::Scheduler(std::vector<Types::JobPriority>& configuration, std::string id) : _threadMap(), _currentWorkItemId(0),
    _running(true), _id(std::move(id))
    {
        for(Types::JobPriority priority : configuration)
        {
            this->_threadMap.insert(std::pair<Types::JobPriority,
                Concurrency::ThreadBasePtr>(priority, std::make_shared<Concurrency::WorkerThread>()));
        }
    }

    Scheduler::~Scheduler()
    {
        if (this->_running)
        {
            this->Shutdown();
        }
    }

	std::map<Types::JobPriority, Concurrency::ThreadBasePtr>& Scheduler::GetThreadMap()
	{
		return this->_threadMap;
	}

	bool Scheduler::ScheduleWorkItem(ExecutableWorkItemBasePtr workItem)
	{
		if (workItem)
		{
			WorkItemBasePtr castedWorkItem = std::dynamic_pointer_cast<WorkItemBase>(workItem);
			castedWorkItem->SetId(++this->_currentWorkItemId);

            auto index = this->GetThreadMap().find(castedWorkItem->GetPriority());
            if (index == this->GetThreadMap().end())
            {
                return false;
            }

            Types::Result_t result = index->second->Queue(workItem);
			//castedWorkItem->Trigger(Types::Result_t::SUCCESS);
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
            for(auto it = this->_threadMap.begin(); it != this->_threadMap.end(); ++it)
            {
                it->second->Stop();
            }
            for (auto it = this->_threadMap.begin(); it != this->_threadMap.end(); ++it)
            {
                // std::cout << "Joining down WorkerThread" << std::endl;
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

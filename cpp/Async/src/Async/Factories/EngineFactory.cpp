// SYSTEM INCLUDES
#include <atomic>
#include <iostream>
#include <mutex>
#include <stdexcept>

// C++ PROJECT INCLUDES
#include "Async/IEngine.hpp"
#include "Async/Engine.hpp"
#include "Async/Async.hpp"

namespace Async
{

std::mutex             engineStartedLock;
EntryPoint::IEnginePtr pEngine = nullptr;
std::atomic<bool>      engineStarted(false);

    // MODEL AS A READER WRITER PROBLEM????

    bool Start(std::string configPath)
    {
        std::lock_guard<std::mutex> lock(engineStartedLock);
        if(engineStarted)
        {
            return false;
        }
        pEngine = std::make_shared<EntryPoint::Engine>(configPath);
        engineStarted = true;
        return engineStarted;
    }

    EntryPoint::IEnginePtr GetEngineSingleton()
    {
        bool val = false;
        {
            std::lock_guard<std::mutex> lock(engineStartedLock);
            val = engineStarted;
        }
        if(!val)
        {
            return nullptr;
        }
        else if (val && !pEngine)
        {
            throw std::logic_error("Engine started but is null");
        }
        return pEngine;
    }

    Types::Result_t SubmitEngineSingletonServiceRequest(IWorkItemPtr pWorkItem,
                                                        std::string schedulerName)
    {
        EntryPoint::IEnginePtr pEngine = GetEngineSingleton();
        std::cout << "Submitting service request" << std::endl;
        if(!pEngine || !pEngine->IsRunning())
        {
            std::cout << "engine is null or not running. Cannot submit request" << std::endl;
            return Types::Result_t::FAILURE;
        }
        ISchedulerPtr pScheduler = pEngine->GetScheduler(schedulerName);
        std::cout << "Got Scheduler from Engine" << std::endl;
        if(!pScheduler)
        {
            std::logic_error("Scheduler is null but Engine is not null");
        }
        std::cout << "Scheduling WorkItem" << std::endl;
        return pWorkItem->Schedule(pScheduler);
    }

    bool Stop()
    {
        std::cout << "Async::Stop() called" << std::endl;
        bool val = false;
        {
            std::lock_guard<std::mutex> lock(engineStartedLock);
            val = engineStarted;
        }
        if(!val)
        {
            return false;
        }
        pEngine->Shutdown();
        pEngine = nullptr;
        engineStarted = false;
        return true;
    }

} // end of namespace Async

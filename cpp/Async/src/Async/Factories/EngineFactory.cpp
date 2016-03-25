// SYSTEM INCLUDES
#include <atomic>
#include <mutex>
#include <stdexcept>

// C++ PROJECT INCLUDES
#include "Async/IEngine.hpp"
#include "Async/Engine.hpp"
#include "Async/Async.hpp"

namespace Async
{

std::mutex             engineLock;
EntryPoint::IEnginePtr pEngine = nullptr;
std::atomic<bool>      engineStarted(false);

    // MODEL AS A READER WRITER PROBLEM????

    bool Start(std::string configPath)
    {
        std::lock_guard<std::mutex> lock(engineLock);
        if(engineStarted)
        {
            return false;
        }
        pEngine = std::make_shared<EntryPoint::Engine>(configPath);
        engineStarted = true;
        return engineStarted;
    }

    EntryPoint::IEnginePtr GetEngineSingletonHelper()
    {
        if (engineStarted && !pEngine)
        {
            throw std::logic_error("Engine started but is null");
        }
        return pEngine;
    }

    EntryPoint::IEnginePtr GetEngineSingleton()
    {
        std::lock_guard<std::mutex> lock(engineLock);
        return GetEngineSingletonHelper();
    }

    Types::Result_t SubmitEngineSingletonServiceRequest(IWorkItemPtr pWorkItem,
                                                        std::string schedulerName)
    {
        std::lock_guard<std::mutex> lock(engineLock);
        EntryPoint::IEnginePtr pEngine = GetEngineSingletonHelper();
        if(!pEngine)
        {
            return Types::Result_t::FAILURE;
        }
        ISchedulerPtr pScheduler = pEngine->GetScheduler(schedulerName);
        if(!pScheduler)
        {
            std::logic_error("Scheduler is null put Engine is not null");
        }
        return pWorkItem->Schedule(pScheduler);
    }

    bool Stop()
    {
        std::lock_guard<std::mutex> lock(engineLock);
        if(!engineStarted)
        {
            return false;
        }
        pEngine->Shutdown();
        pEngine = nullptr;
        engineStarted = false;
        return true;
    }

} // end of namespace Async

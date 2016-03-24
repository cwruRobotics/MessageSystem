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
std::atomic<bool>      engineStarted = false;

    bool Start(std::string configPath)
    {
        std::lock_guard<std::mutex> lock(engineLock);
        if(engineStarted)
        {
            return false;
        }
        engineStarted = true;
        pEngine = std::make_shared<EntryPoint::Engine>(configPath);
        return engineStarted;
    }

    EntryPoint::IEnginePtr GetEngineSingleton()
    {
        if (engineStarted && !pEngine)
        {
            throw std::logic_error("Engine started but is null");
        }
        return pEngine;
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

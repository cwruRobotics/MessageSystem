// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/Engine.hpp"
#include "Async/Scheduler.hpp"
#include "Async/JobPriorities.hpp"

namespace Async
{
namespace EntryPoint
{

    Engine::Engine(std::string configPath) : _schedulerMap(), _schedulerMapMutex(), _running(true)
    {
        if (configPath != "")
        {
            this->Configure(configPath);
        }
    }

    Engine::~Engine()
    {
        if (this->_running)
        {
            this->Shutdown();
        }
    }

    void Engine::Configure(std::string& configPath)
    {
        using ConfigMapType = std::map<std::string, std::vector<Types::JobPriority> >;
        // check validity of file path, open if it exists, and try to parse out configurations
        // for Async. Pass down these configurations to each Scheduler.
        ConfigMapType configuration;

        // check file path validity
        // open file and parse

        for(ConfigMapType::iterator it = configuration.begin(); it != configuration.end(); ++it)
        {
            ISchedulerPtr pScheduler = std::make_shared<Scheduler>(it->second, it->first);
            this->_schedulerMap.insert(std::pair<std::string, ISchedulerPtr>(it->first, pScheduler));
        }
    }

    std::map<std::string, ISchedulerPtr>& Engine::GetSchedulerMap()
    {
        return this->_schedulerMap;
    }

    ISchedulerPtr Engine::FindScheduler(std::string& schedulerId)
    {
        auto index = this->GetSchedulerMap().find(schedulerId);
        if (index == this->GetSchedulerMap().end())
        {
            return nullptr;
        }
        return index->second;
    }

    ISchedulerPtr Engine::GetScheduler(std::string& schedulerId)
    {
        if (this->_running)
        {
            std::lock_guard<std::mutex> mapLock(this->_schedulerMapMutex);
            auto index = this->GetSchedulerMap().find(schedulerId);
            if (index == this->GetSchedulerMap().end())
            {
                return nullptr;
            }
            return index->second;
        }
        return nullptr;
    }

    Types::Result_t Engine::ShutdownScheduler(std::string& schedulerId)
    {
        if (this->_running)
        {
            std::lock_guard<std::mutex> mapLock(this->_schedulerMapMutex);
            ISchedulerPtr pScheduler = this->FindScheduler(schedulerId);
            if (pScheduler)
            {
                pScheduler->Shutdown();
            }
            return Types::Result_t::SUCCESS;
        }
        return Types::Result_t::FAILURE;
    }

    Types::Result_t Engine::StartScheduler(std::string& schedulerId, std::vector<Types::JobPriority>& config)
    {
        if (this->_running)
        {
            ISchedulerPtr pScheduler = std::make_shared<Scheduler>(config, schedulerId);

            std::lock_guard<std::mutex> mapLock(this->_schedulerMapMutex);
            this->GetSchedulerMap().insert(std::pair<std::string&,
                ISchedulerPtr>(schedulerId, pScheduler));
            return Types::Result_t::SUCCESS;
        }
        return Types::Result_t::FAILURE;
    }

    Types::Result_t Engine::Shutdown()
    {
        std::lock_guard<std::mutex> mapLock(this->_schedulerMapMutex);

        this->_running = false;
        for (auto it = this->GetSchedulerMap().begin(); it != this->GetSchedulerMap().end(); ++it)
        {
            it->second->Shutdown();
        }
        return Types::Result_t::SUCCESS;
    }

} // end of namespace EntryPoint
} // end of namespace Async

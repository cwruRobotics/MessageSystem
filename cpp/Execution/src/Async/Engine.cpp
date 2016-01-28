// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/Engine.hpp"
#include "Async/Scheduler.hpp"

namespace Async
{
namespace EntryPoint
{

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

    void Engine::BuildConfiguration(const char* pConfigFilePath)
    {
        return;
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

    Types::Result_t Engine::StartScheduler(std::string& schedulerId)
    {
        if (this->_running)
        {
            ISchedulerPtr pScheduler = std::make_shared<Scheduler>(schedulerId);

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

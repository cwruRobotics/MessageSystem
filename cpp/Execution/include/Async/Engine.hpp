#pragma once
#ifndef ASYNC_ENTRYPOINT_ENGINE_HPP
#define ASYNC_ENTRYPOINT_ENGINE_HPP

// SYSTEM INCLUDES
#include <map>
#include <mutex>

// C++ PROJECT INCLUDES
#include "Async/IEngine.hpp"
#include "Async/IScheduler.hpp"


namespace Async
{
namespace EntryPoint
{

    class Engine : public IEngine
    {
    private:

        std::map<std::string, ISchedulerPtr>    _schedulerMap;
        std::mutex                              _schedulerMapMutex;
        std::atomic<bool>                       _running;

    private:

        std::map<std::string, ISchedulerPtr>& GetSchedulerMap();

        ISchedulerPtr FindScheduler(std::string& schedulerId);

    public:
        Engine() : _schedulerMap(), _schedulerMapMutex(), _running(true)
        {
        }

        ~Engine()
        {
            if (this->_running)
            {
                this->Shutdown();
            }
        }

        Engine(const Engine& other) = delete;

        void operator=(const Engine& other) = delete;

        void BuildConfiguration(const char* pConfigFilePath) override;

        ISchedulerPtr GetScheduler(std::string& schedulerId) override;

        Types::Result_t ShutdownScheduler(std::string& schedulerId) override;

        Types::Result_t StartScheduler(std::string& schedulerId) override;

        Types::Result_t Shutdown() override;

    };

    using EnginePtr = std::shared_ptr<Engine>;

} // end of namespace EntryPoint
} // end of namespace Async


#endif // end of header guard

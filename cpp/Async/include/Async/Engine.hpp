#pragma once
#ifndef ASYNC_ENTRYPOINT_ENGINE_HPP
#define ASYNC_ENTRYPOINT_ENGINE_HPP

// SYSTEM INCLUDES
#include <map>
#include <mutex>

// C++ PROJECT INCLUDES
#include "Async/EngineBase.hpp"
#include "Async/SchedulerBase.hpp"


namespace Async
{
namespace EntryPoint
{

    class Engine : public EngineBase
    {
    private:

        std::map<std::string, SchedulerBasePtr> _schedulerMap;
        std::mutex                              _schedulerMapMutex;
        std::atomic<bool>                       _running;

    private:

        void Configure(std::string& configPath);

        std::map<std::string, SchedulerBasePtr>& GetSchedulerMap();

        SchedulerBasePtr FindScheduler(std::string& schedulerId);

    public:

        Engine(std::string configPath);

        ~Engine();

        Engine(const Engine& other) = delete;

        void operator=(const Engine& other) = delete;

        SchedulerBasePtr GetScheduler(std::string& schedulerId) override;

        Types::Result_t ShutdownScheduler(std::string& schedulerId) override;

        Types::Result_t StartScheduler(std::string& schedulerId,
                                       std::vector<Types::JobPriority> config) override;

        Types::Result_t Shutdown() override;

        bool IsRunning() override;

    };

    using EnginePtr = std::shared_ptr<Engine>;

} // end of namespace EntryPoint
} // end of namespace Async


#endif // end of header guard

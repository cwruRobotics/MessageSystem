#pragma once
#ifndef ASYNC_ENTRYPOINT_IENGINE_HPP
#define ASYNC_ENTRYPOINT_IENGINE_HPP

// SYSTEM INCLUDES
#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

// C++ PROJECT INCLUDES
#include "Async/LibraryExport.hpp"
#include "Async/Result.hpp"
#include "Async/SchedulerBase.hpp"
#include "Async/WorkItemBase.hpp"

namespace Async
{
namespace EntryPoint
{

    class EngineBase;
    using EngineBasePtr = std::shared_ptr<EngineBase>;

    class EngineBase
    {
    public:

        virtual ~EngineBase() = default;

        virtual SchedulerBasePtr GetScheduler(std::string& schedulerId) = 0;

        virtual Types::Result_t ShutdownScheduler(std::string& schedulerId) = 0;

        virtual Types::Result_t StartScheduler(std::string& schedulerId,
                                               std::vector<Types::JobPriority> config) = 0;

        virtual Types::Result_t Shutdown() = 0;

        virtual bool IsRunning() = 0;

    };

} // end of namespace EntryPoint

    ASYNC_API EntryPoint::EngineBasePtr GetEngineSingleton();

    ASYNC_API Types::Result_t SubmitEngineSingletonServiceRequest(WorkItemBasePtr pWorkItem,
                                                                  std::string schedulerName);

} // end of namespace Async

#endif

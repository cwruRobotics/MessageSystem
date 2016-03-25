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
#include "Async/IScheduler.hpp"
#include "Async/IWorkItem.hpp"

namespace Async
{
namespace EntryPoint
{

    class IEngine;
    using IEnginePtr = std::shared_ptr<IEngine>;

    class IEngine
    {
    public:

        virtual ~IEngine() = default;

        virtual ISchedulerPtr GetScheduler(std::string& schedulerId) = 0;

        virtual Types::Result_t ShutdownScheduler(std::string& schedulerId) = 0;

        virtual Types::Result_t StartScheduler(std::string& schedulerId,
                                               std::vector<Types::JobPriority> config) = 0;

        virtual Types::Result_t Shutdown() = 0;

    };

} // end of namespace EntryPoint

    ASYNC_API EntryPoint::IEnginePtr GetEngineSingleton();

    ASYNC_API Types::Result_t SubmitEngineSingletonServiceRequest(IWorkItemPtr pWorkItem,
                                                                  std::string schedulerName);

} // end of namespace Async

#endif

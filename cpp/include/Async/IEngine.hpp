#pragma once
#ifndef ASYNC_ENTRYPOINT_IENGINE_HPP
#define ASYNC_ENTRYPOINT_IENGINE_HPP

// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES
#include "Async/LibraryExport.hpp"
#include "Async/Result.hpp"
#include "Async/IScheduler.hpp"
#include "Async/Promise.hpp"

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

        virtual void BuildConfiguration(const char* pConfigFilePath) = 0;

        virtual ISchedulerPtr GetScheduler(std::string& schedulerId) = 0;

        virtual Types::Result_t ShutdownScheduler(std::string& schedulerId) = 0;

        virtual Types::Result_t StartScheduler(std::string& schedulerId) = 0;

        virtual Types::Result_t Shutdown() = 0;

    };

} // end of namespace EntryPoint

    ASYNC_API EntryPoint::IEnginePtr GetStaticEngine();

} // end of namespace Async

#endif

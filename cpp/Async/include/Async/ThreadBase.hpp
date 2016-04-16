#pragma once

#ifndef ASYNC_ITHREAD_HPP
#define ASYNC_ITHREAD_HPP

// SYSTEM INCLUDES
#include <thread>

// C++ PROJECT INCLUDES
#include "Async/Result.hpp"
#include "Async/ConcurrencyStates.hpp"
#include "Async/ExecutableWorkItemBase.hpp"

// project namespace
namespace Async
{
// component namespace
namespace Concurrency
{

    class ThreadBase
    {
    public:
        virtual ~ThreadBase() = default;

        virtual std::thread::id GetId() = 0;

        virtual States::ConcurrencyState GetState() = 0;

        virtual Types::Result_t Queue(ExecutableWorkItemBasePtr workItem) = 0;

        virtual void Stop() = 0;

        virtual void Join() = 0;

        virtual Types::Result_t Abort() = 0;

        virtual void Run() = 0;
    };

    using ThreadBasePtr = std::shared_ptr<ThreadBase>;

} // end of namespace Concurrency
} // end of namespace Async


#endif

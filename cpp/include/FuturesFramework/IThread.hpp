#pragma once

#ifndef FUTURESFRAMEWORK_ITHREAD_HPP
#define FUTURESFRAMEWORK_ITHREAD_HPP

// SYSTEM INCLUDES
#include <thread>

// C++ PROJECT INCLUDES
#include "FuturesFramework/Result.hpp"
#include "FuturesFramework/ConcurrencyStates.hpp"
#include "FuturesFramework/IExecutableWorkItem.hpp"

// project namespace
namespace FuturesFramework
{
// component namespace
namespace Concurrency
{

    class IThread
    {
    public:
        virtual ~IThread() = default;

        virtual std::thread::id GetId() = 0;

        virtual States::ConcurrencyState GetState() = 0;

        virtual Types::Result_t Queue(IExecutableWorkItemPtr workItem) = 0;

        virtual void Join() = 0;

        virtual Types::Result_t Abort() = 0;

        virtual void Run() = 0;
    };

    using IThreadPtr = std::unique_ptr<IThread>;

} // end of namespace Concurrency
} // end of namespace FuturesFramework


#endif

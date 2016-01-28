#pragma once

#ifndef ASYNC_TESTS_MOCKSCHEDULER_HPP
#define ASYNC_TESTS_MOCKSCHEDULER_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/Scheduler.hpp"

namespace Async
{
namespace Tests
{

    class MockScheduler : public Scheduler
    {
    public:
        MockScheduler() : Scheduler()
        {
        }

        ~MockScheduler()
        {
        }

        std::map<Types::JobPriority, Concurrency::IThreadPtr>& GetThreadMap();
    };

} // end of namespace Tests

using MockSchedulerPtr = std::shared_ptr<Tests::MockScheduler>;

} // end of namespace Async

#endif

#pragma once

#ifndef ASYNC_TESTS_MOCKSCHEDULER_HPP
#define ASYNC_TESTS_MOCKSCHEDULER_HPP

// SYSTEM INCLUDES
#include <vector>


// C++ PROJECT INCLUDES
#include "Async/Scheduler.hpp"
#include "Async/JobPriorities.hpp"

namespace Async
{
namespace Tests
{

    class MockScheduler : public Scheduler
    {
    public:
        MockScheduler(std::vector<Types::JobPriority> config) : Scheduler(config, "mockScheduler")
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

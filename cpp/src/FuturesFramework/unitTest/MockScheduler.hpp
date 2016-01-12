#pragma once

#ifndef FUTURESFRAMEWORK_TESTS_MOCKSCHEDULER_HPP
#define FUTURESFRAMEWORK_TESTS_MOCKSCHEDULER_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/Scheduler.hpp"

namespace FuturesFramework
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

        std::map<uint64_t, IExecutableWorkItemPtr>& GetWorkItemMap();

        std::map<Types::JobPriority, Concurrency::IThreadPtr>& GetThreadMap();

        bool DetachWorkItem(uint64_t id);
    };

} // end of namespace Tests

using MockSchedulerPtr = std::shared_ptr<Tests::MockScheduler>;

} // end of namespace FuturesFramework

#endif

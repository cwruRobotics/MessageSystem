
// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/unitTest/MockScheduler.hpp"

namespace FuturesFramework
{
namespace Tests
{

    std::map<uint64_t, IExecutableWorkItemPtr>& MockScheduler::GetWorkItemMap()
    {
        return this->Scheduler::GetWorkItemMap();
    }

    std::map<Types::JobPriority, Concurrency::IThreadPtr>& MockScheduler::GetThreadMap()
    {
        return this->Scheduler::GetThreadMap();
    }

    bool MockScheduler::DetachWorkItem(uint64_t id)
    {
        return this->Scheduler::DetachWorkItem(id);
    }

} // end of namespace Tests
} // end of namespace FuturesFramework

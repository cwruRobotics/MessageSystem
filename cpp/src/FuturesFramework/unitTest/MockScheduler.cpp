
// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/unitTest/MockScheduler.hpp"

namespace FuturesFramework
{
namespace Tests
{

    std::map<uint64_t, IWorkItemPtr>& 
        MockScheduler::GetWorkItemMap()
    {
        return this->Scheduler::GetWorkItemMap();
    }

    std::map<std::thread::id, std::thread>& 
        MockScheduler::GetThreadMap()
    {
        return this->Scheduler::GetThreadMap();
    }

    bool MockScheduler::DetachWorkItem(uint64_t id)
    {
        return this->Scheduler::DetachWorkItem(id);
    }

    bool MockScheduler::ExecuteWorkItem(const uint64_t id)
    {
        return this->Scheduler::ExecuteWorkItem(id);
    }

} // end of namespace Tests
} // end of namespace FuturesFramework

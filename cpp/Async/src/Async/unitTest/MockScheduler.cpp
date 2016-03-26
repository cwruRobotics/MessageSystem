
// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/unitTest/MockScheduler.hpp"

namespace Async
{
namespace Tests
{

    std::map<Types::JobPriority, Concurrency::ThreadBasePtr>& MockScheduler::GetThreadMap()
    {
        return this->Scheduler::GetThreadMap();
    }

} // end of namespace Tests
} // end of namespace Async

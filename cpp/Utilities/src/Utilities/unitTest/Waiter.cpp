// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Utilities/unitTest/Waiter.hpp"


namespace Utilities
{
namespace Tests
{

    void Waiter(CommonMemoryPtr pCommon)
    {
        pCommon->_semaphore.wait();
    }

} // end of namespace Tests
} // end of namespace Structures

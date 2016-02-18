// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Utilities/unitTest/Signaler.hpp"


namespace Utilities
{
namespace Tests
{

    void Signaler(CommonMemoryPtr& pCommon)
    {
        pCommon->_semaphore.signal();
    }

} // end of namespace Tests
} // end of namespace Utilities

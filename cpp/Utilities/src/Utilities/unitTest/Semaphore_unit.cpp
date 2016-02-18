// SYSTEM INCLUDES
#include <thread>


// C++ PROJECT INCLUDES
#include "catch/catch.hpp"

#include "Utilities/unitTest/SimpleSemaphoreTest.hpp"

namespace Utilities
{
namespace Tests
{

    TEST_CASE("Test simple semaphore test", "[Semaphore_unit]")
    {
        SimpleSemaphoreTest(12);
    }

} // end of namespace Tests
} // end of namespace Utilities

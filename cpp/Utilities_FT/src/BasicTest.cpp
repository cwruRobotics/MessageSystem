
// SYSTEM INCLUDES
#include <gtest/gtest.h>
#include <Utilities/Semaphore.hpp>

// C++ PROJECT INCLUDES


namespace Utilities_FT
{

    TEST(SemaphoreTest, SimpleTest)
    {
        Utilities::Semaphore sem(1);
        EXPECT_EQ(1, 1);
    }

} // end of Utilities_FT


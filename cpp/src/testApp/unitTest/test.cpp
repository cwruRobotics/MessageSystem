// SYSTEM INCLUDES

// C++ PROJECT INCLUDES
#include "gtest/gtest.h"
#include "testApp/testInclude.hpp"

TEST(TestReturn5, Test)
{
    EXPECT_EQ(5, return5());
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

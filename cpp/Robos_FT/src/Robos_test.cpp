// SYSTEM INCLUDES
#include <gtest/gtest.h>
#include <Utilities/OSUtils.hpp>
#include <Robos/Robos.hpp>


// C++ PROJECT INCLUDES


namespace OSUtils = Utilities::OS;

namespace Robos_FT
{

    TEST(RobosSimpleTest, TrivialTest)
    {
        ASSERT_EQ(1, 1);
    }

} // end of namespace Robos_FT

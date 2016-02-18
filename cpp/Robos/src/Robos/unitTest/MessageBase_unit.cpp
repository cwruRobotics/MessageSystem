// SYSTEM INCLUDES
#include <string>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"

#include "Robos/MessageBase.hpp"

namespace Robos
{
namespace Tests
{

    TEST_CASE("Testing Topic assignment", "[MessageBase_unit]")
    {
        std::string testTopic = "testTopic"; 
        MessageBase mb(testTopic);

        REQUIRE( mb.topic == testTopic );
    }

} // end of namespace Tests
} // end of namespace Robos

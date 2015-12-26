// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "testApp/unitTest/catchPort.hpp"
#include "testApp/testInclude.hpp"

TEST_CASE("Testing simple Program", "[return5]")
{
    REQUIRE( return5() == 5 );
}

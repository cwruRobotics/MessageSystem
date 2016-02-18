// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "catch/catch.hpp"

#include "Robos/NodeDatabase.hpp"

namespace Robos
{
namespace Tests
{

    TEST_CASE("Since NodeDatabase is so simple, all that is required is testing Instantiation",
        "[NodeDatabase_unit]")
    {
        Internal::NodeDatabase db;

        REQUIRE( db.map.size() == 0 );
    }

} // end of namespace Tests
} // end of namespace Robos

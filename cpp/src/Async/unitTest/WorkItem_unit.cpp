// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "Async/Result.hpp"
#include "Async/WorkItem.hpp"

namespace Async
{
namespace Tests
{

    TEST_CASE("Testing WorkItem default constructor", "[WorkItem_unit]")
    {
        uint64_t customId = 15;
        WorkItem defaultWItem, customWItem(customId);

        REQUIRE( defaultWItem.GetId() == 0 );
        REQUIRE( customWItem.GetId() == customId );
    }

    TEST_CASE("Testing AttachMainFunction", "[WorkItem_unit]")
    {
        WorkItem workItem;

        try
        {
            workItem.AttachMainFunction([]() -> Types::Result_t
            {
                return Types::Result_t::SUCCESS;
            });

            // no way to check internal member variable.
            // this test passes if no exception is thrown..I guess
            REQUIRE( true );
        }
        catch(...)
        {
            // if exception is thrown fail test.
            REQUIRE( false );
        }
    }

    TEST_CASE("Testing AttachPosteriorFunction", "[WorkItem_unit]")
    {
        WorkItem workItem;

        try
        {
            workItem.AttachPosteriorFunction([]() -> Types::Result_t
            {
                    return Types::Result_t::SUCCESS;
            });

            REQUIRE( true );
        }
        catch(...)
        {
            // if exception is thrown fail test
            REQUIRE( false );
        }
    }

    TEST_CASE("Testing GetException", "[WorkItem_unit]")
    {
        WorkItem workItem;

        REQUIRE( workItem.GetException() == nullptr );
    }

} // end of namespace Tests
} // end of namespace Async

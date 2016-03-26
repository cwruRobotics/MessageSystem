// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "Async/unitTest/TestUtilities.hpp"
#include "Async/Result.hpp"
#include "Async/WorkItemStates.hpp"
#include "Async/unitTest/MockWorkItem.hpp"

namespace Async
{
namespace Tests
{

    TEST_CASE("Testing WorkItem default constructor", "[WorkItem_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("WorkItem_unit test 1");
        uint64_t customId = 15;
        MockWorkItem defaultWItem, customWItem(customId);

        REQUIRE( defaultWItem.GetId() == 0 );
        REQUIRE( customWItem.GetId() == customId );
    }

    TEST_CASE("Testing AttachMainFunction", "[WorkItem_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("WorkItem_unit test 2");
        MockWorkItem workItem;

        try
        {
            workItem.AttachMainFunction([]() -> States::WorkItemState
            {
                return States::WorkItemState::DONE;
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
        Utilities::WriteUnitTestNameToConsole("WorkItem_unit test 3");
        MockWorkItem workItem;

        try
        {
            workItem.AttachPosteriorFunction([]() -> States::WorkItemState
            {
                    return States::WorkItemState::DONE;
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
        Utilities::WriteUnitTestNameToConsole("WorkItem_unit test 4");
        MockWorkItem workItem;

        REQUIRE( workItem.GetException() == nullptr );
    }

} // end of namespace Tests
} // end of namespace Async

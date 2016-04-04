// SYSTEM INCLUDES
#include <memory>


// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "Async/unitTest/TestUtilities.hpp"

#include "Async/ValueHolder.hpp"


namespace Async
{
namespace Tests
{

    TEST_CASE("Instantiating ValueHolder", "[ValueHolder_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("ValueHolder_unit test 1");
        ValueHolder<int> intValue;
        ValueHolder<double> doubleValue;
        ValueHolder<std::shared_ptr<int> > intPtrValue;
    }

    TEST_CASE("Assigning values to ValueHolder", "[ValueHolder_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("ValueHolder_unit test 2");
        ValueHolder<int> intValue;

        intValue.Assign(5);
        REQUIRE( intValue );
        REQUIRE( intValue.HasValue() );
        REQUIRE( intValue.GetValue() == 5 );
        REQUIRE( intValue.GetPointer() != nullptr );
        REQUIRE( *intValue.GetPointer() == 5 );
    }

} // end of namespace Tests
} // end of namespace Async
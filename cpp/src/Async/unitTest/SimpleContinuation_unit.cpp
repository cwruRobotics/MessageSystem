// SYSTEM INCLUDES
#include <chrono>
#include <iostream>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"

#include "Async/AsyncExecution.hpp"
#include "Async/SettlementStates.hpp"
#include "Async/Promise.hpp"

namespace Async
{
namespace Tests
{

    TEST_CASE("Testing SimpleContinuation of Promises", "[SimpleContinuation_unit]")
    {
        std::string schedulerId = "testScheduler";
        PromisePtr<int> pPromise = Execute<int>([]() -> int
        {
            return 10;
        }, schedulerId);
        PromisePtr<int> pSuccessor = pPromise->Then<int>([](int a) -> int
        {
            return a + 5;
        }, schedulerId);

        std::cout << "Sleeping for 2 seconds to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));

        REQUIRE( pPromise->GetState() == States::SettlementState::SUCCESS );
        REQUIRE( pPromise->GetResult() == 10 );

        REQUIRE( pSuccessor->GetState() == States::SettlementState::SUCCESS );
        REQUIRE( pSuccessor->GetResult() == 15 );
    }

} // end of namespace Tests
} // end of namespace Async

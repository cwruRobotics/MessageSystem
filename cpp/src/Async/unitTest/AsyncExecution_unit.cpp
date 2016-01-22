// SYSTEM INCLUDES
#include <chrono>
#include <iostream>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"

#include "Async/SettlementStates.hpp"
#include "Async/Promise.hpp"
#include "Async/IEngine.hpp"
#include "Async/Scheduler.hpp"
#include "Async/AsyncExecution.hpp"

namespace Async
{
namespace Tests
{

    TEST_CASE("Testing AsyncExecution::Execute()", "[AsyncExecution_unit]")
    {
        std::string schedulerId = "testScheduler";
        PromisePtr<int> pPromise = Execute<int>([]() -> int
        {
            return 10;
        }, schedulerId);
        EntryPoint::IEnginePtr pEngine = GetStaticEngine();
        REQUIRE( pEngine != nullptr );
        REQUIRE( pEngine->GetScheduler(schedulerId) != nullptr );
        REQUIRE( pEngine->GetScheduler(schedulerId)->IsRunning() );

        std::cout << "Sleeping for 1 second to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        REQUIRE( pPromise->GetState() == States::SettlementState::SUCCESS );
        REQUIRE( pPromise->GetResult() == 10 );
    }

} // end of namespace Tests
} // end of namespace Async

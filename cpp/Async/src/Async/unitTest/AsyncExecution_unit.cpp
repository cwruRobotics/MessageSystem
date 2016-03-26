// SYSTEM INCLUDES
#include <chrono>
#include <iostream>
#include <Utilities/OSUtils.hpp>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "Async/unitTest/TestUtilities.hpp"

#include "Async/SettlementStates.hpp"
#include "Async/Promise.hpp"
#include "Async/EngineBase.hpp"
#include "Async/Scheduler.hpp"
#include "Async/Async.hpp"
#include "Async/AsyncExecution.hpp"

namespace OSUtils = Utilities::OS;

namespace Async
{
namespace Tests
{

    TEST_CASE("Testing AsyncExecution::Execute()", "[AsyncExecution_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("AsyncExecution_unit test 1");
        std::string configFilePath = OSUtils::GetCurrentDirectory(__FILE__) +
            OSUtils::GetPathSep() + "TestEngineConfig.xml";
        Start(configFilePath);

        std::string schedulerId = "TestScheduler";
        PromisePtr<int> pPromise = Execute<int>([]() -> int
        {
            return 10;
        }, schedulerId);

        std::cout << "Checking Engine Singleton" << std::endl;
        EntryPoint::EngineBasePtr pEngine = GetEngineSingleton();
        REQUIRE( pEngine != nullptr );
        REQUIRE( pEngine->GetScheduler(schedulerId) != nullptr );
        REQUIRE( pEngine->GetScheduler(schedulerId)->IsRunning() );

        std::cout << "\tSleeping for 1 second to allow execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        REQUIRE( pPromise->GetState() == States::SettlementState::SUCCESS );
        REQUIRE( pPromise->GetResult() == 10 );

        Stop();
    }

} // end of namespace Tests
} // end of namespace Async

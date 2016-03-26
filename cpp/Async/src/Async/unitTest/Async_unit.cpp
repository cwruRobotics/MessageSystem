// SYSTEM INCLUDES
#include <stdexcept>
#include <vector>
#include <Utilities/OSUtils.hpp>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "Async/unitTest/TestUtilities.hpp"

#include "Async/Result.hpp"
#include "Async/EngineBase.hpp"
#include "Async/Engine.hpp"
#include "Async/Async.hpp"


#include "AsyncLoggingConfig.hpp"

namespace OSUtils = Utilities::OS;

namespace Async
{
namespace Tests
{

    TEST_CASE("Checking that by default, the engine singleton is null", "[Async_unit]")
    {
        REQUIRE( GetEngineSingleton() == nullptr );
    }

    TEST_CASE("Starting and stopping the engine singleton", "[Async_unit]")
    {
        std::string configFilePath = OSUtils::GetCurrentDirectory(__FILE__) +
            OSUtils::GetPathSep() + "TestEngineConfig.xml";
        REQUIRE( Start(configFilePath) );
        REQUIRE( GetEngineSingleton() != nullptr );

        std::vector<std::string> schedulersToCheck = Utilities::GetSchedulersToCheckFor(configFilePath);
        for(std::string schedulerName : schedulersToCheck)
        {
            REQUIRE( GetEngineSingleton()->GetScheduler(schedulerName) != nullptr );
            REQUIRE( GetEngineSingleton()->GetScheduler(schedulerName)->IsRunning() );
        }

        REQUIRE( Stop() );
    }

    TEST_CASE("Calling Async::Start() multiple times", "[Async_unit]")
    {
        std::string configFilePath = OSUtils::GetCurrentDirectory(__FILE__) +
            OSUtils::GetPathSep() + "TestEngineConfig.xml";
        REQUIRE( Start(configFilePath) );
        REQUIRE( GetEngineSingleton() != nullptr );

        for(int i = 0; i < 10; ++i)
        {
            REQUIRE( !Start(configFilePath) );
        }

        REQUIRE( Stop() );
    }

    TEST_CASE("Calling Async::Stop() multiple times", "[Async_unit]")
    {
        std::string configFilePath = OSUtils::GetCurrentDirectory(__FILE__) +
            OSUtils::GetPathSep() + "TestEngineConfig.xml";
        REQUIRE( Start(configFilePath) );
        REQUIRE( GetEngineSingleton() != nullptr );

        REQUIRE( Stop() );
        for(int i = 0; i < 10; ++i)
        {
            REQUIRE( !Stop() );
        }
    }

    TEST_CASE("Calling Async::GetEngineSingleton() before Async::Start() or after Async::Stop() should throw",
        "[Async_unit]")
    {
        REQUIRE( GetEngineSingleton() == nullptr );

        std::string configFilePath = OSUtils::GetCurrentDirectory(__FILE__) +
            OSUtils::GetPathSep() + "TestEngineConfig.xml";
        REQUIRE( Start(configFilePath) );
        REQUIRE( GetEngineSingleton() != nullptr );
        REQUIRE( Stop() );

        REQUIRE( GetEngineSingleton() == nullptr );
    }

} // end of namespace Tests
} // end of namespace Async

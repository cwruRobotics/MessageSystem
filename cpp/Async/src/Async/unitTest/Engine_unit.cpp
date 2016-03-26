// SYSTEM INCLUDES
#include <vector>
#include <Logging/Factory.hpp>
#include <Logging/ILogger.hpp>
#include <Utilities/OSUtils.hpp>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "Async/unitTest/TestUtilities.hpp"

#include "Async/Result.hpp"
#include "Async/JobPriorities.hpp"
#include "Async/EngineBase.hpp"
#include "Async/Engine.hpp"
#include "Async/Async.hpp"

#include "AsyncLoggingConfig.hpp"

namespace OSUtils = Utilities::OS;

namespace Async
{
namespace Tests
{

    TEST_CASE("Testing default Engine constructor and destructor", "[Engine_unit]")
    {
        std::string loggingPath = Async::LoggingConfig::LOGGING_ROOT + "/Engine_unitLog.txt";
        Logging::LoggerPtr pLogger = Logging::Factory::MakeLogger("Engine_unitLogger",
            loggingPath.c_str());
        LOG_DEBUG(pLogger, "Testing default Engine constructor and destructor");
        Utilities::WriteUnitTestNameToConsole("Engine_unit test 1");
        try
        {
            // test 1
            LOG_DEBUG(pLogger, "Parsing file with 1 Scheduler");
            std::string configFilePath = OSUtils::GetCurrentDirectory(__FILE__) +
                OSUtils::GetPathSep() + "TestEngineConfig.xml";
            EntryPoint::Engine engine(configFilePath);
            std::vector<std::string> schedulersToCheck = Utilities::GetSchedulersToCheckFor(configFilePath);
            for(std::string schedulerName : schedulersToCheck)
            {
                LOG_DEBUG(pLogger, "\tScheduler name %s", schedulerName.c_str());
                REQUIRE( engine.GetScheduler(schedulerName) != nullptr );
                REQUIRE( engine.GetScheduler(schedulerName)->IsRunning() );
            }

            // test 2
            LOG_DEBUG(pLogger, "Parsing file with 2 Schedulers");
            configFilePath = OSUtils::GetCurrentDirectory(__FILE__) +
                OSUtils::GetPathSep() + "TestEngineConfig.xml";
            EntryPoint::Engine engine2(configFilePath);
            schedulersToCheck = Utilities::GetSchedulersToCheckFor(configFilePath);
            for(std::string schedulerName : schedulersToCheck)
            {
                LOG_DEBUG(pLogger, "\tScheduler name %s", schedulerName.c_str());
                REQUIRE( engine2.GetScheduler(schedulerName) != nullptr );
                REQUIRE( engine2.GetScheduler(schedulerName)->IsRunning() );
            }

            // test many
            LOG_DEBUG(pLogger, "Parsing file with 10 Schedulers");
            configFilePath = OSUtils::GetCurrentDirectory(__FILE__) +
                OSUtils::GetPathSep() + "TestEngineConfig.xml";
            EntryPoint::Engine engine10(configFilePath);
            schedulersToCheck = Utilities::GetSchedulersToCheckFor(configFilePath);
            for(std::string schedulerName : schedulersToCheck)
            {
                LOG_DEBUG(pLogger, "\tScheduler name %s", schedulerName.c_str());
                REQUIRE( engine10.GetScheduler(schedulerName) != nullptr );
                REQUIRE( engine10.GetScheduler(schedulerName)->IsRunning() );
            }
        }
        catch(std::exception e)
        {
            LOG_ERROR(pLogger, "Error: %s", e.what());
            REQUIRE( false );
        }

    }

    TEST_CASE("Testing Engine::Shutdown() with no Schedulers", "[Engine_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("Engine_unit test 2");
        std::string configFilePath = OSUtils::GetCurrentDirectory(__FILE__) +
            OSUtils::GetPathSep() + "TestEngineConfig.xml";
        EntryPoint::Engine engine(configFilePath);

        std::vector<std::string> schedulersToCheck = Utilities::GetSchedulersToCheckFor(configFilePath);
        for(std::string schedulerName : schedulersToCheck)
        {
            REQUIRE( engine.GetScheduler(schedulerName) != nullptr );
            REQUIRE( engine.GetScheduler(schedulerName)->IsRunning() );
        }
        REQUIRE( engine.Shutdown() == Types::Result_t::SUCCESS );
    }

    TEST_CASE("Testing Engine::StartScheduler() and Engine::GetScheduler()", "[Engine_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("Engine_unit test 3");
        SchedulerBasePtr pScheduler = nullptr;
        {
            std::string configFilePath = OSUtils::GetCurrentDirectory(__FILE__) +
                OSUtils::GetPathSep() + "TestEngineConfig.xml";
            EntryPoint::Engine engine(configFilePath);
            std::vector<std::string> schedulersToCheck = Utilities::GetSchedulersToCheckFor(configFilePath);
            for(std::string schedulerName : schedulersToCheck)
            {
                REQUIRE( engine.GetScheduler(schedulerName) != nullptr );
                REQUIRE( engine.GetScheduler(schedulerName)->IsRunning() );
            }

            std::string schedulerId = "testScheduler";
            REQUIRE( engine.StartScheduler(schedulerId, Utilities::GetDefaultSchedulerConfig())
                == Types::Result_t::SUCCESS );

            pScheduler = engine.GetScheduler(schedulerId);
            if (pScheduler != nullptr)
            {
                REQUIRE( pScheduler->IsRunning() );
            }
            else
            {
                REQUIRE( false );
            }
        }
        REQUIRE( !pScheduler->IsRunning() );
    }

    TEST_CASE("Testing Engine::ShutdownScheduler()", "[Engine_unit]")
    {
        Utilities::WriteUnitTestNameToConsole("Engine_unit test 4");
        std::string configFilePath = OSUtils::GetCurrentDirectory(__FILE__) +
            OSUtils::GetPathSep() + "TestEngineConfig.xml";
        EntryPoint::Engine engine(configFilePath);

        std::vector<std::string> schedulersToCheck = Utilities::GetSchedulersToCheckFor(configFilePath);
        for(std::string schedulerName : schedulersToCheck)
        {
            REQUIRE( engine.GetScheduler(schedulerName) != nullptr );
            REQUIRE( engine.GetScheduler(schedulerName)->IsRunning() );
        }

        std::string schedulerId = "testScheduler";
        REQUIRE( engine.StartScheduler(schedulerId, Utilities::GetDefaultSchedulerConfig())
            == Types::Result_t::SUCCESS );
        REQUIRE( engine.GetScheduler(schedulerId)->IsRunning() );
        REQUIRE( engine.ShutdownScheduler(schedulerId) == Types::Result_t::SUCCESS );
        REQUIRE( !engine.GetScheduler(schedulerId)->IsRunning() );
    }

} // end of namespace Tests
} // end of namespace Async

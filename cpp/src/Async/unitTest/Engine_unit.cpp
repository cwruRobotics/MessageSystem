// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "catch/catch.hpp"

#include "Async/Result.hpp"
#include "Async/IEngine.hpp"
#include "Async/Engine.hpp"

namespace Async
{
namespace Tests
{

    TEST_CASE("Testing default Engine constructor and destructor", "[Engine_unit]")
    {
        try
        {
            EntryPoint::Engine engine;
            REQUIRE( true );
        }
        catch(...)
        {
            REQUIRE( false );
        }

    }

    TEST_CASE("Testing Engine::Shutdown() with no Schedulers", "[Engine_unit]")
    {
        EntryPoint::Engine engine;
        REQUIRE( engine.Shutdown() == Types::Result_t::SUCCESS );
    }

    TEST_CASE("Testing Engine::StartScheduler() and Engine::GetScheduler()", "[Engine_unit]")
    {
        ISchedulerPtr pScheduler = nullptr;
        {
            EntryPoint::Engine engine;
            std::string schedulerId = "testScheduler";

            REQUIRE( engine.StartScheduler(schedulerId) == Types::Result_t::SUCCESS );

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
        EntryPoint::Engine engine;
        std::string schedulerId = "testScheduler";

        REQUIRE( engine.StartScheduler(schedulerId) == Types::Result_t::SUCCESS );
        REQUIRE( engine.GetScheduler(schedulerId)->IsRunning() );
        REQUIRE( engine.ShutdownScheduler(schedulerId) == Types::Result_t::SUCCESS );
        REQUIRE( !engine.GetScheduler(schedulerId)->IsRunning() );
    }

} // end of namespace Tests
} // end of namespace Async

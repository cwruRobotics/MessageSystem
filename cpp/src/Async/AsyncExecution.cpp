// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/Result.hpp"
#include "Async/Scheduler.hpp"
#include "Async/Engine.hpp"
#include "Async/AsyncExecution.hpp"


namespace Async
{

    EntryPoint::IEnginePtr GetStaticEngine()
    {
        static EntryPoint::IEnginePtr pEngine = std::make_shared<EntryPoint::Engine>();

        return pEngine;
    }

    EntryPoint::IEnginePtr GetEngine()
    {
        return GetStaticEngine();
    }

}

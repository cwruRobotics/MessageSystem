// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/IEngine.hpp"
#include "Async/Engine.hpp"

namespace Async
{

    EntryPoint::IEnginePtr GetStaticEngine()
    {
        static EntryPoint::IEnginePtr pEngine = std::make_shared<EntryPoint::Engine>();

        return pEngine;
    }

} // end of namespace Async

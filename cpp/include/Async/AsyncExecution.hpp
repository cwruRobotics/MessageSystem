#ifndef ASYNC_ASYNCEXECUTION_HPP
#define ASYNC_ASYNCEXECUTION_HPP

// SYSTEM INCLUDES
#include <functional>

// C++ PROJECT INCLUDES
#include "Async/LibraryExport.hpp"
#include "Async/Promise.hpp"
#include "Async/IScheduler.hpp"
#include "Async/IEngine.hpp"

namespace Async
{


    template<typename PROMISE_RESULT> 
    PromisePtr<PROMISE_RESULT> Execute(std::function<PROMISE_RESULT()> pFunc,
                                       std::string& schedulerId)
    {
        EntryPoint::IEnginePtr pEngine = GetStaticEngine();
        ISchedulerPtr pScheduler = pEngine->GetScheduler(schedulerId);
        Types::Result_t result = Types::Result_t::SUCCESS;
        if (!pScheduler)
        {
            result = pEngine->StartScheduler(schedulerId);
        }
        if (result == Types::Result_t::FAILURE)
        {
            return nullptr;
        }
        pScheduler = pEngine->GetScheduler(schedulerId);
        PromisePtr<PROMISE_RESULT> pPromise = std::make_shared<Promise<PROMISE_RESULT> >();
        pPromise->AttachMainFunction(pFunc);
        pPromise->Schedule(pScheduler);
        return pPromise;
    }

} // end of namespace Async

#endif // end of ASYNC_ASYNCEXECUTION_HPP

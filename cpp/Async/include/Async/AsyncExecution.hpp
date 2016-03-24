#ifndef ASYNC_ASYNCEXECUTION_HPP
#define ASYNC_ASYNCEXECUTION_HPP

// SYSTEM INCLUDES
#include <stdexcept>
#include <functional>

// C++ PROJECT INCLUDES
#include "Async/LibraryExport.hpp"
#include "Async/Result.hpp"
#include "Async/JobPriorities.hpp"
#include "Async/Promise.hpp"
#include "Async/IScheduler.hpp"
#include "Async/IEngine.hpp"

namespace Async
{


    template<typename PROMISE_RESULT> 
    PromisePtr<PROMISE_RESULT> Execute(std::function<PROMISE_RESULT()> pFunc,
                                       std::string& schedulerId,
                                       Types::JobPriority priority=Types::JobPriority::OTHER)
    {
        PromisePtr<PROMISE_RESULT> pPromise = std::make_shared<Promise<PROMISE_RESULT> >(priority);
        pPromise->MakeSafe();
        pPromise->AttachMainFunction(pFunc);
        if(pPromise->Schedule(GetEngineSingleton()
           ->GetScheduler(schedulerId)) != Types::Result_t::SUCCESS)
        {
            throw std::runtime_error("Error scheduling Promise");
        }
        return pPromise;
    }

} // end of namespace Async

#endif // end of ASYNC_ASYNCEXECUTION_HPP

#pragma once

#ifndef ASYNC_FT_PROMISEFACTORY_HPP
#define ASYNC_FT_PROMISEFACTORY_HPP

// SYSTEM INCLUDES
#include <functional>
#include <Async/JobPriorities.hpp>
#include <Async/AsyncExecution.hpp>

// C++ PROJECT INCLUDES


namespace Async_FT
{
namespace PromiseFactory
{

    template<typename PROMISE_RESULT>
    void ExecuteAsynchronously(std::function<PROMISE_RESULT()> pFunc,
                               std::string schedulerName,
                               Async::Types::JobPriority priority=Async::Types::JobPriority::OTHER)
    {
        Async::Execute<PROMISE_RESULT>(pFunc, schedulerName, priority);
    }

} // end of namespace PromiseFactory
} // end of namespace Async_FT

#endif // end of ASYNC_FT_PROMISEFACTORY_HPP

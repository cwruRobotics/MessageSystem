#pragma once

#ifndef ASYNC_IPROMISE_HPP
#define ASYNC_IPROMISE_HPP

// SYSTEM INCLUDES
#include <functional>
#include <memory>

// C++ PROJECT INCLUDES
// #include "Async/LibraryExport.hpp"
#include "Async/Result.hpp"

// project namespace
namespace Async
{

    // interface for client usage of Promises.
    // This interface excludes the Then() function
    // because that method itself is templated (different
    // from the class template information), and you
    // cannot have a virtual template method.
    // This interface is exported to clients.
    template<typename PROMISE_RESULT>
    class /*ASYNC_API*/ PromiseBase
    {
    protected:
        // virtual void SetMainFunction(
        //     std::function<Types::Result_t()> pFunc) = 0;

    public:

        virtual ~PromiseBase()
        {
        }

        virtual PROMISE_RESULT GetResult() = 0;

        // this is public so that clients can write Promises
        // that do NOT take advantage of the concurrency part
        // of this framework. A Promise can be fulfilled
        // through client code.
        virtual void Fulfill(PROMISE_RESULT result) = 0;
    };

    // void template specialization.
    // this template specialization is exported to clients.
    template<>
    class /*ASYNC_API*/ PromiseBase<void>
    {
    protected:
        // virtual void SetFunction(
        //     std::function<Types::Result_t()> pFunc) = 0;

    public:
        virtual ~PromiseBase() = default;

        virtual std::exception_ptr GetError() = 0;

        virtual void Fulfill() = 0;
    };

    // alias for shared pointer to an IPromise instance.
    template<typename PROMISE_RESULT>
    using PromiseBasePtr = std::shared_ptr<PromiseBase<PROMISE_RESULT> >;

} // end of namespace Async

#endif // end of header guard

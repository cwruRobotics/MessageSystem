#pragma once

#ifndef FUTURESFRAMEWORK_IPROMISE_HPP
#define FUTURESFRAMEWORK_IPROMISE_HPP

// SYSTEM INCLUDES
#include <functional>
#include <memory>

// C++ PROJECT INCLUDES
#include "FuturesFramework/LibraryExport.hpp"
#include "FuturesFramework/Result.hpp"

// project namespace
namespace FuturesFramework
{

    // interface for client usage of Promises.
    // This interface excludes the Then() function
    // because that method itself is templated (different
    // from the class template information), and you
    // cannot have a virtual template method.
    // This interface is exported to clients.
    template<typename PROMISE_RESULT>
    class FUTURESFRAMEWORK_API IPromise
    {
    protected:
        // virtual void SetMainFunction(
        //     std::function<Types::Result_t()> pFunc) = 0;

    public:
        virtual ~IPromise() = default;

        virtual PROMISE_RESULT GetResult() = 0;

        virtual std::exception_ptr GetError() = 0;

        // this is public so that clients can write Promises
        // that do NOT take advantage of the concurrency part
        // of this framework. A Promise can be fulfilled
        // through client code.
        virtual void Fulfill(PROMISE_RESULT result) = 0;
    };

    // void template specialization.
    // this template specialization is exported to clients.
    template<>
    class FUTURESFRAMEWORK_API IPromise<void>
    {
    protected:
        // virtual void SetFunction(
        //     std::function<Types::Result_t()> pFunc) = 0;

    public:
        virtual ~IPromise() = default;

        virtual std::exception_ptr GetError() = 0;

        virtual void Fulfill() = 0;
    };

    // alias for shared pointer to an IPromise instance.
    template<typename PROMISE_RESULT>
    using IPromisePtr = std::shared_ptr<IPromise<PROMISE_RESULT> >;

} // end of namespace FuturesFramework

#endif // end of header guard

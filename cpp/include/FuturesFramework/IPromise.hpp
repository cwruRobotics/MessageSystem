#pragma once

#ifndef FUTURESFRAMEWORK_IPROMISE_HPP
#define FUTURESFRAMEWORK_IPROMISE_HPP

// SYSTEM INCLUDES
#include <functional>
#include <memory>

// C++ PROJECT INCLUDES
#include "FuturesFramework/LibraryExport.hpp"
#include "FuturesFramework/Result.hpp"

namespace FuturesFramework
{
    template<typename PROMISE_RESULT>
    class FUTURESFRAMEWORK_API IPromise
    {
    protected:
        virtual void SetMainFunction(
            std::function<Types::Result_t()> pFunc) = 0;

    public:
        virtual ~IPromise() = default;

        virtual PROMISE_RESULT GetResult() = 0;

        virtual std::exception_ptr GetError() = 0;

        virtual void Fulfill(PROMISE_RESULT result) = 0;
    };

    template<>
    class FUTURESFRAMEWORK_API IPromise<void>
    {
    protected:
        virtual void SetFunction(
            std::function<Types::Result_t()> pFunc) = 0;

    public:
        virtual ~IPromise() = default;

        virtual std::exception_ptr GetError() = 0;

        virtual void Fulfill() = 0;
    };

    template<typename PROMISE_RESULT>
    using IPromisePtr = std::shared_ptr<IPromise<PROMISE_RESULT> >;
}


#endif

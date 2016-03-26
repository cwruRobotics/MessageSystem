#pragma once

#ifndef ASYNC_PROMISELIFESAVER_HPP
#define ASYNC_PROMISELIFESAVER_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/IChainLinker.hpp"

namespace Async
{

    template<typename PROMISE_RESULT>
    class Promise;

    template<typename PROMISE_RESULT>
    class PromiseLifeSaver : public IChainLinker
    {
    private:

        std::shared_ptr<Promise<PROMISE_RESULT> > _pPromise;

    protected:

        virtual Types::Result_t ApplyFunctionToChild() override
        {
            return Types::Result_t::SUCCESS;
        }

    public:

        PromiseLifeSaver(std::shared_ptr<Promise<PROMISE_RESULT> > pPromise) :
            _pPromise(pPromise)
        {
        }

        ~PromiseLifeSaver()
        {
            this->_pPromise = nullptr;
        }

        virtual Types::Result_t Chain() override
        {
            return Types::Result_t::SUCCESS;
        }

    };

    template<typename PROMISE_RESULT>
    using PromiseLifeSaverPtr = std::shared_ptr<PromiseLifeSaver<PROMISE_RESULT> >;

} // end of namespace Async

#endif // end of ASYNC_PROMISELIFESAVER_HPP

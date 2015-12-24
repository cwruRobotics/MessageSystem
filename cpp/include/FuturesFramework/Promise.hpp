#pragma once

#ifndef FUTURESFRAMEWORK_PROMISE_HPP
#define FUTURESFRAMEWORK_PROMISE_HPP

// SYSTEM INCLUDES

// C++ PROJECT INCLUDES
#include "FuturesFramework/ChainLinker.hpp"
#include "FuturesFramework/IPromise.hpp"
#include "FuturesFramework/LibraryExport.hpp"
#include "FuturesFramework/PromiseBase.hpp"

namespace FuturesFramework
{
    
    template<typename PROMISERESULT>
    class Promise;
    
    template<typename PROMISERESULT>
    using PromisePtr = std::shared_ptr<Promise<PROMISERESULT> >;
    
    template<typename PROMISERESULT>
    class Promise
    {
    };
    
    template<typename PROMISE_RESULT, typename ... ARGS>
    class Promise<PROMISE_RESULT(ARGS...)> : public IPromise<PROMISE_RESULT>,
        public PromiseBase,
        public std::enable_shared_from_this<Promise<PROMISE_RESULT(ARGS...)> >
    {
    private:

        PROMISE_RESULT                          _result;
        std::function<PROMISE_RESULT(ARGS...)>  _unboundFunction;
        bool                                    _argsGiven;

    public:
        Promise() : PromiseBase(), _cachedArgs(), _unboundFunction(),
            _argsGiven(false)
        {
        }

        ~Promise()
        {
            this->_unboundFunction = nullptr;
            this->_cachedArgs = nullptr;
            this->_argsGiven = false;
        }

        void Fulfill(PROMISE_RESULT result) override
        {
            this->_internalWorkItem->SetSuccess();
            this->_result = result;
            this->SetState(States::SettlementState::SUCCESS);
        }

        PROMISE_RESULT GetResult() override
        {
            if (this->GetState() != States::SettlementState::SUCCESS)
            {
                std::exception_ptr toThrow = this->GetError();
                if (toThrow)
                {
                    std::rethrow_exception(toThrow);
                }
                else
                {
                    throw std::exception("No failure but also no error?");
                }
            }
            return this->_result;
        }

        std::exception_ptr GetError() override
        {
            return this->_internalWorkItem->GetException();
        }

        bool PreconditionsMet()
        {
            return this->_argsGiven;
        }

        void GiveArgs(ARGS... args)
        {
            if (!this->_argsGiven)
            {
                if (this->GetState() == States::SettlementState::SUCCESS)
                {
                    return;
                }
                this->_argsGiven = true;

                // using bindType =
                //     decltype(std::bind(std::declval<std::function<PROMISE_RESULT(ARGS...)> >(),
                //         std::declval<ARGS>()...));

                // bindType boundFunction = std::bind(this->_unboundFunction,
                //     std::forward<ARGS>(args)...);

                auto boundFunction = std::bind(this->_unboundFunction,
                    std::forward<ARGS>(args)...);
                std::dynamic_pointer_cast<IWorkItem>(this->_internalWorkItem)
                    ->AttachMainFunction([this, boundFunction]() ->Types::Result_t
                {
                    if (this->PreconditionsMet())
                    {
                        this->Fulfill(boundFunction());
                        return Types::Result_t::SUCCESS;
                    }
                    return Types::Result_t::FAILURE;
                });
            }
        }

        void AttachMainFunction(std::function<PROMISE_RESULT(ARGS...)> pFunc)
        {
            this->_unboundFunction = std::move(pFunc);
        }

    };
    
}


#endif

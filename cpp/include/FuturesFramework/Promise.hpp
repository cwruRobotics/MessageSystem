#pragma once

#ifndef FUTURESFRAMEWORK_PROMISE_HPP
#define FUTURESFRAMEWORK_PROMISE_HPP

// SYSTEM INCLUDES

// C++ PROJECT INCLUDES
// #include "FuturesFramework/ChainLinker.hpp"
#include "FuturesFramework/IPromise.hpp"
#include "FuturesFramework/LibraryExport.hpp"
#include "FuturesFramework/PromiseBase.hpp"

namespace FuturesFramework
{
    
    template<typename PROMISE_RESULT>
    class Promise;
    
    template<typename PROMISE_RESULT, typename... ARGS>
    using PromisePtr = std::shared_ptr<Promise<PROMISE_RESULT(ARGS...)> >;
    
    //template<typename PROMISERESULT>
    //class Promise
    //{ 
    //};
    
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
        Promise() : PromiseBase(), _unboundFunction(),
            _argsGiven(false)
        {
        }

        ~Promise()
        {
            this->_unboundFunction = nullptr;
            this->_argsGiven = false;
        }

        void Fulfill(PROMISE_RESULT result) override
        {
            this->_internalWorkItem->SetSuccess();
            this->_result = result;
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
                    throw std::exception("Promise not settled yet");
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
                if (this->GetState() == States::SettlementState::SUCCESS
                    || this->GetState() == States::SettlementState::FAILURE
                    || !this->_unboundFunction)
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
                std::dynamic_pointer_cast<IExecutableWorkItem>(this->_internalWorkItem)
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
            if (!this->_unboundFunction)
            {
                std::dynamic_pointer_cast<IExecutableWorkItem>(this->_internalWorkItem)
                    ->AttachMainFunction([]() -> Types::Result_t
                {
                    return Types::Result_t::FAILURE;
                });
                this->_unboundFunction = std::move(pFunc);
            }
        }

    };
    
}


#endif

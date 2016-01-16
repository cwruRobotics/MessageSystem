#pragma once

#ifndef ASYNC_PROMISE_HPP
#define ASYNC_PROMISE_HPP

// SYSTEM INCLUDES
#include <atomic>

// C++ PROJECT INCLUDES
#include "Async/SimpleChainLinker.hpp"
#include "Async/IPromise.hpp"
#include "Async/LibraryExport.hpp"
#include "Async/PromiseBase.hpp"

namespace Async
{

    template<typename PROMISE_RESULT>
    class Promise;

    template<typename PROMISE_RESULT>
    using PromisePtr = std::shared_ptr<Promise<PROMISE_RESULT> >;

    template<typename PROMISE_RESULT>
    class Promise : public PromiseBase, public IPromise<PROMISE_RESULT>
    {
    private:

        PROMISE_RESULT  _result;

    public:
        Promise() : PromiseBase()
        {
        }

        ~Promise()
        {
        }

        void Fulfill(PROMISE_RESULT result)
        {
            this->_internalWorkItem->SetSuccess();
            this->_result = result;
        }

        PROMISE_RESULT GetResult() override
        {
            if (this->GetState() != States::SettlementState::SUCCESS)
            {
                std::exception_ptr pException = this->GetError();
                if (pException)
                {
                    std::rethrow_exception(pException);
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

        void AttachMainFunction(std::function<PROMISE_RESULT()> pFunc)
        {
            std::dynamic_pointer_cast<IExecutableWorkItem>(this->_internalWorkItem)
                ->AttachMainFunction([this, pFunc]() -> Types::Result_t
            {
                this->Fulfill(pFunc());
                return Types::Result_t::SUCCESS;
            });
        }

        template<typename NEXT_RESULT>
        PromisePtr<NEXT_RESULT> Then(std::function<NEXT_RESULT(PROMISE_RESULT)> pFunc,
            std::string& childSchedulerId);
    };

// #include "Async/SimpleChainLinker.hpp"
template<typename PROMISE_RESULT>
template<typename NEXT_RESULT>
PromisePtr<NEXT_RESULT> Promise<PROMISE_RESULT>::Then(std::function<NEXT_RESULT(PROMISE_RESULT)> pFunc,
    std::string& childSchedulerId)
{
    PromisePtr<NEXT_RESULT> pSuccessor = std::make_shared<Promise<NEXT_RESULT> >();
    IChainLinkerPtr pChain =
        std::make_shared<SimpleChainLinker<PROMISE_RESULT, NEXT_RESULT> >(
            std::dynamic_pointer_cast<Promise<PROMISE_RESULT> >(shared_from_this()),
            pSuccessor,
            pFunc,
            childSchedulerId
        );
    this->AddSuccessor(pChain, true);
    if (this->GetState() == States::SettlementState::SUCCESS)
    {
        pChain->Chain();
    }
    return pSuccessor;
}

    
    template<typename PROMISE_RESULT, typename ARG1, typename ... ARGS>
    class Promise<PROMISE_RESULT(ARG1, ARGS...)> : public IPromise<PROMISE_RESULT>,
        public PromiseBase,
        public std::enable_shared_from_this<Promise<PROMISE_RESULT(ARG1, ARGS...)> >
    {
    private:

        PROMISE_RESULT                                  _result;
        std::function<PROMISE_RESULT(ARG1, ARGS...)>    _unboundFunction;
        std::atomic<bool>                               _argsGiven;

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

        void GiveArgs(ARG1 arg1, ARGS... args)
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

                std::lock_guard<std::mutex> executionLock(this->_executionMutex);
                auto boundFunction = std::bind(this->_unboundFunction, arg1,
                    std::forward<ARGS>(args)...);
                std::dynamic_pointer_cast<IExecutableWorkItem>(this->_internalWorkItem)
                    ->AttachMainFunction([this, boundFunction]() ->Types::Result_t
                {
                    std::lock_guard<std::mutex> executionLock(this->_executionMutex);
                    if (this->PreconditionsMet())
                    {
                        this->Fulfill(boundFunction());
                        return Types::Result_t::SUCCESS;
                    }
                    return Types::Result_t::FAILURE;
                });
            }
        }

        void AttachMainFunction(std::function<PROMISE_RESULT(ARG1, ARGS...)> pFunc)
        {
            if (!this->_unboundFunction)
            {
                std::dynamic_pointer_cast<IExecutableWorkItem>(this->_internalWorkItem)
                    ->AttachMainFunction([this]() -> Types::Result_t
                {
                    std::lock_guard<std::mutex> executionLock(this->_executionMutex);
                    return Types::Result_t::FAILURE;
                });
                this->_unboundFunction = std::move(pFunc);
            }
        }        

    };
    
}


#endif

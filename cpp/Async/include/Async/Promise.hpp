#pragma once

#ifndef ASYNC_PROMISE_HPP
#define ASYNC_PROMISE_HPP

// SYSTEM INCLUDES
#include <atomic>
#include <stdexcept>

// C++ PROJECT INCLUDES
#include "Async/JobPriorities.hpp"
#include "Async/SimpleChainLinker.hpp"
//#include "Async/PromiseLifeSaver.hpp"
#include "Async/PromiseBase.hpp"
// #include "Async/LibraryExport.hpp"
#include "Async/PromiseBaseImpl.hpp"
#include "Async/ValueHolder.hpp"

namespace Async
{

    template<typename PROMISE_RESULT>
    class Promise;

    template<typename PROMISE_RESULT>
    using PromisePtr = std::shared_ptr<Promise<PROMISE_RESULT> >;

    template<typename PROMISE_RESULT>
    class /*ASYNC_API*/ Promise : public PromiseBase<PROMISE_RESULT>, public PromiseBaseImpl
    {
    private:

        ValueHolderPtr<PROMISE_RESULT>  _resultHolder;

    public:
        Promise(Types::JobPriority priority=Types::JobPriority::OTHER) :    PromiseBaseImpl(priority),
            _resultHolder(std::make_shared<ValueHolder<PROMISE_RESULT> >())
        {
        }

        ~Promise()
        {
        }

        void Fulfill(PROMISE_RESULT result)
        {
            this->SetSuccess();
            this->_resultHolder->Assign(std::forward<PROMISE_RESULT>(result));
        }

        PROMISE_RESULT GetResult() override
        {
            if (this->GetState() != States::SettlementState::SUCCESS)
            {
                std::exception_ptr pException = this->GetException();
                if (pException)
                {
                    std::rethrow_exception(pException);
                }
                else
                {
                    throw std::logic_error("Promise not settled yet");
                }
            }
            return this->_resultHolder->GetValue();
        }

        void AttachMainFunction(std::function<PROMISE_RESULT()> pFunc)
        {
            /*
                There is a really, really, really subtle program error here.

                A Promise instance so far is not kept alive BY Async until it is executed,
                so the scenario exists where, if client code does not "save" the Promise
                instance, it will be destructed (using a shared_ptr) when the client code
                exits IF the client code takes less time than it does for Async to fulfill
                the Promise. Then, since we are capturing by reference in the lambda, the
                client function may not exist anymore AND the Promise instance may not exist
                anymore.
            */

            // get a copy to the Promise that is being executed here. This should prevent
            // the Promise instance from destructing (because the counter increments by 1)...I think
            // PromisePtr<PROMISE_RESULT> thisCopy = this->shared_from_this();
            this->PromiseBaseImpl::AttachMainFunction([this, pFunc]() -> States::WorkItemState
            {
                // convert it to a Promise. This should prevent the Promise instance from being
                // destructed.
                this->Fulfill(pFunc());
                return States::WorkItemState::DONE;
            });
        }

        template<typename NEXT_RESULT>
        PromisePtr<NEXT_RESULT> Then(std::function<NEXT_RESULT(PROMISE_RESULT)> pFunc,
            std::string& childSchedulerId)
        {
            PromisePtr<NEXT_RESULT> pSuccessor = std::make_shared<Promise<NEXT_RESULT> >(this->GetPriority());

            // this is a safe way to transfer the result of the parent Promise to
            // the child Promise
            auto pContinuationFunction = [this, pFunc]() -> std::function<NEXT_RESULT()>
            {
                auto pValueHolder = this->_resultHolder->shared_from_this();
                return [pValueHolder, pFunc]() -> NEXT_RESULT
                {
                    return pFunc(pValueHolder->GetValue());
                };
            };

            // create the linker to be executed on success of this Promise
            ChainLinkerBasePtr pChain =
                std::make_shared<SimpleChainLinker<PROMISE_RESULT, NEXT_RESULT> >(
                    pContinuationFunction,
                    pSuccessor,
                    childSchedulerId
                );
            /*
            IChainLinkerPtr pChain =
                std::make_shared<SimpleChainLinker<PROMISE_RESULT, NEXT_RESULT> >(
                    this,
                    pFunc,
                    pSuccessor->shared_from_this(),
                    childSchedulerId
                );
            */

            // "true" for "execute on success"
            this->AddContinuation(pChain, true);

            // if the Promise has already resolved successfully, execute the chain
            if (this->GetState() == States::SettlementState::SUCCESS)
            {
                pChain->Chain();
            }
            return pSuccessor;
        }
    };

    /*
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
            this->SetSuccess();
            this->_result = result;
        }

        PROMISE_RESULT GetResult() override
        {
            if (this->GetState() != States::SettlementState::SUCCESS)
            {
                std::exception_ptr toThrow = this->GetException();
                if (toThrow)
                {
                    std::rethrow_exception(toThrow);
                }
                else
                {
                    throw std::logic_error("Promise not settled yet");
                }
            }
            return this->_result;
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

                auto boundFunction = std::bind(this->_unboundFunction, arg1,
                    std::forward<ARGS>(args)...);
                this->AttachMainFunction([this, boundFunction]() -> States::WorkItemState
                {
                    if (this->PreconditionsMet())
                    {
                        this->Fulfill(boundFunction());
                        return States::WorkItemState::DONE;
                    }
                    return States::WorkItemState::DONE;
                });
            }
        }

        void AttachMainFunction(std::function<PROMISE_RESULT(ARG1, ARGS...)> pFunc)
        {
            if (!this->_unboundFunction)
            {
                this->AttachMainFunction([this]() -> States::WorkItemState
                {
                    return States::WorkItemState::DONE;
                });
                this->_unboundFunction = std::move(pFunc);
            }
        }        

    };
    */
    
}


#endif

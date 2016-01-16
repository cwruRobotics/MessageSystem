#ifndef ASYNC_SIMPLECHAINLINKER_HPP
#define ASYNC_SIMPLECHAINLINKER_HPP

// SYSTEM INCLUDES
#include <functional>


// C++ PROJECT INCLUDES
#include "Async/IChainLinker.hpp"

namespace Async
{

    template<typename PROMISE_RESULT>
    class Promise;


    template<typename PARENT_TYPE, typename CHILD_TYPE>
    class SimpleChainLinker : public IChainLinker,
        public std::enable_shared_from_this<SimpleChainLinker<PARENT_TYPE, CHILD_TYPE> >
    {
    private:

        std::shared_ptr<Promise<PARENT_TYPE> >  _pParent;
        std::shared_ptr<Promise<CHILD_TYPE> >   _pChild;
        std::function<CHILD_TYPE(PARENT_TYPE)>  _pChildFunction;
        std::string                             _childSchedulerId;

    protected:

        virtual Types::Result_t ApplyFunctionToChild() override;

    public:

        SimpleChainLinker(std::shared_ptr<Promise<PARENT_TYPE> > pParent,
            std::shared_ptr<Promise<CHILD_TYPE> > pChild,
            std::function<CHILD_TYPE(PARENT_TYPE)> pFunction, std::string id) : _pParent(pParent),
            _pChild(pChild), _pChildFunction(pFunction), _childSchedulerId(id)
        {
        }

        ~SimpleChainLinker()
        {
        }

        virtual Types::Result_t Chain() override;
    };

    template<typename PARENT_TYPE, typename CHILD_TYPE>
    using SimpleChainLinkerPtr = std::shared_ptr<SimpleChainLinker<PARENT_TYPE, CHILD_TYPE> >;

#include "Async/Promise.hpp"
// #include "Async/AsyncExecution.hpp"

    template<typename PARENT_TYPE, typename CHILD_TYPE>
    Types::Result_t SimpleChainLinker<PARENT_TYPE, CHILD_TYPE>::ApplyFunctionToChild()
    {
        PARENT_TYPE value = this->_pParent->GetResult();
        std::function<CHILD_TYPE(PARENT_TYPE)> pFunc = this->_pChildFunction;
        this->_pChild->AttachMainFunction([value, pFunc]() -> CHILD_TYPE
        {
            return pFunc(value);
        });
        //if (!GetEngine()->GetScheduler(this->_childSchedulerId))
        //{
        //    GetEngine()->StartScheduler(this->_childSchedulerId);
        //}
        //this->_pChild->Schedule(GetEngine()->GetScheduler(this->_childSchedulerId));
        this->_pParent = nullptr;
        return Types::Result_t::SUCCESS;
    }

    template<typename PARENT_TYPE, typename CHILD_TYPE>
    Types::Result_t SimpleChainLinker<PARENT_TYPE, CHILD_TYPE>::Chain()
    {
        return this->ApplyFunctionToChild();
    }

} // end of namespace Async

#endif // end of ASYNC_SIMPLECHAINLINKER_HPP

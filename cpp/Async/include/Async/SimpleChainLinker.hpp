#ifndef ASYNC_SIMPLECHAINLINKER_HPP
#define ASYNC_SIMPLECHAINLINKER_HPP

// SYSTEM INCLUDES
#include <functional>


// C++ PROJECT INCLUDES
#include "Async/IChainLinker.hpp"
#include "Async/IEngine.hpp"

namespace Async
{

    template<typename PROMISE_RESULT>
    class Promise;


    template<typename PARENT_TYPE, typename CHILD_TYPE>
    class SimpleChainLinker : public IChainLinker,
        public std::enable_shared_from_this<SimpleChainLinker<PARENT_TYPE, CHILD_TYPE> >
    {
    private:

        // std::function<std::function<CHILD_TYPE()>()>    _pResolutionFunction;
        std::function<CHILD_TYPE(PARENT_TYPE)>          _pChildFunction;
        std::shared_ptr<Promise<PARENT_TYPE> >          _pParent;
        std::shared_ptr<Promise<CHILD_TYPE> >           _pChild;
        std::string                                     _childSchedulerId;

    protected:

        virtual Types::Result_t ApplyFunctionToChild() override;

    public:

        SimpleChainLinker(// std::function<std::function<CHILD_TYPE()>()> pResolutionFunction,
            std::shared_ptr<Promise<PARENT_TYPE> > pParent,
            std::function<CHILD_TYPE(PARENT_TYPE)> pChildFunction,
            std::shared_ptr<Promise<CHILD_TYPE> > pChild, std::string id) : _pChild(pChild),
            // _pResolutionFunction(pResolutionFunction),
            _childSchedulerId(id),
            _pParent(pParent), _pChildFunction(pChildFunction)
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

    template<typename PARENT_TYPE, typename CHILD_TYPE>
    Types::Result_t SimpleChainLinker<PARENT_TYPE, CHILD_TYPE>::ApplyFunctionToChild()
    {
        //this->_pChild->AttachMainFunction(this->_pResolutionFunction());
        this->_pChild->AttachMainFunction(std::bind(this->_pChildFunction, this->_pParent->GetResult()));
        EntryPoint::IEnginePtr pEngine = GetEngineSingleton();
        if(pEngine == nullptr)
        {
            return Types::Result_t::FAILURE;
        }
        this->_pChild->Schedule(GetEngineSingleton()->GetScheduler(this->_childSchedulerId));
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

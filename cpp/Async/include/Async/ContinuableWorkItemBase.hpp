#pragma once

#ifndef ASYNC_ICONTINUABLEWORKITEM_HPP
#define ASYNC_ICONTINUABLEWORKITEM_HPP

// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES
#include "Async/LibraryExport.hpp"
#include "Async/ChainLinkerBase.hpp"

// project namespace
namespace Async
{

    // FORWARD DECLARATIONS
    class ContinuableWorkItemBase;
    using ContinuableWorkItemBasePtr =
        std::shared_ptr<ContinuableWorkItemBase>;

    // the interface for a WorkItem that can have
    // successors. This interface is exposed to clients.
    class ASYNC_API ContinuableWorkItemBase
    {
    protected:

        // successfully resolve this IContinuableWorkItem
        virtual void SetSuccess() = 0;

        // unsuccessfully resolve this IContinuableWorkItem
        virtual void SetFailure() = 0;

        // add a successor to this IContinuableWorkItem. A successor
        // will be constructed when this IContinuableWorkItem is resolved.
        virtual void AddContinuation(ChainLinkerBasePtr pContinuation,
            bool onSuccess=true) = 0;


    public:

        virtual ~ContinuableWorkItemBase() = default;

    };

} // end of namespace Async

#endif // end of header guard

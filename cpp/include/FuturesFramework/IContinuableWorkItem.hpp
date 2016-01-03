#pragma once

#ifndef FUTURESFRAMEWORK_ICONTINUABLEWORKITEM_HPP
#define FUTURESFRAMEWORK_ICONTINUABLEWORKITEM_HPP

// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES
#include "FuturesFramework/LibraryExport.hpp"
#include "FuturesFramework/IChainLinker.hpp"

// project namespace
namespace FuturesFramework
{

    // FORWARD DECLARATIONS
    class IContinuableWorkItem;
    using IContinuableWorkItemPtr =
        std::shared_ptr<IContinuableWorkItem>;

    // the interface for a WorkItem that can have
    // successors. This interface is exposed to clients.
    class FUTURESFRAMEWORK_API IContinuableWorkItem
    {

    public:

        virtual ~IContinuableWorkItem() = default;

        // successfully resolve this IContinuableWorkItem
        virtual void SetSuccess() = 0;

        // unsuccessfully resolve this IContinuableWorkItem
        virtual void SetFailure() = 0;

        virtual bool IsCurrentlyExecuting() = 0;

        // add a successor to this IContinuableWorkItem. A successor
        // will be constructed when this IContinuableWorkItem is resolved.
        virtual void AddContinuation(IChainLinkerPtr pContinuation,
            bool onSuccess=true) = 0;

    };

} // end of namespace FuturesFramework

#endif // end of header guard

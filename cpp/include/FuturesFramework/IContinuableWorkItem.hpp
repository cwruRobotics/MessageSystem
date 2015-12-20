#pragma once

#ifndef FUTURESFRAMEWORK_ICONTINUABLEWORKITEM_HPP
#define FUTURESFRAMEWORK_ICONTINUABLEWORKITEM_HPP

// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES
#include "FuturesFramework/LibraryExport.hpp"
#include "FuturesFramework/IChainLinker.hpp"

namespace FuturesFramework
{

    class IContinuableWorkItem;
    using IContinuableWorkItemPtr =
        std::shared_ptr<IContinuableWorkItem>;

    class FUTURESFRAMEWORK_API IContinuableWorkItem
    {

    public:

        virtual ~IContinuableWorkItem() = default;

        virtual void SetSuccess() = 0;

        virtual void SetFailure() = 0;

        virtual bool IsCurrentlyExecuting() = 0;

        virtual void AddContinuation(IChainLinkerPtr pContinuation) = 0;

    };

}

#endif

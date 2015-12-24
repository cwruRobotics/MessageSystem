#pragma once

#ifndef FUTURESFRAMEWORK_IEXECUTABLEWORKITEM_HPP
#define FUTURESFRAMEWORK_IEXECUTABLEWORKITEM_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/IWorkItem.hpp"

namespace FuturesFramework
{
    class IExecutableWorkItem : public IWorkItem
    {
    protected:
        // this is the method that an IScheduler calls to run this
        // IWorkItem. The fact that this is protected is for protection.
        // We don't want clients to have access to the Execute()
        // functionality AND we want to force the client to
        // make an IScheduler (either client created or built in)
        // to be a "friend."
        virtual Types::Result_t Execute() = 0;

    public:

        virtual void AttachMainFunction(FunctionPtr pFunc) = 0;

        virtual void AttachPosteriorFunction(FunctionPtr pFunc) = 0;
    };

    using IExecutableWorkItemPtr = std::shared_ptr<IExecutableWorkItem>;
}


#endif
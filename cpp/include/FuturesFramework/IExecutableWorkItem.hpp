#pragma once

#ifndef FUTURESFRAMEWORK_IEXECUTABLEWORKITEM_HPP
#define FUTURESFRAMEWORK_IEXECUTABLEWORKITEM_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/LibraryExport.hpp"
#include "FuturesFramework/IWorkItem.hpp"

// project namespace
namespace FuturesFramework
{

    // interface to allow IWorkItems to be executable by Schedulers.
    // This interface is exported to clients.
    class FUTURESFRAMEWORK_API IExecutableWorkItem : public IWorkItem
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

        // attach a function to be executed. This function is used to
        // resolve this IExecutableWorkItem
        virtual void AttachMainFunction(FunctionPtr pFunc) = 0;

        // when the main function is done, the client can provide
        // a "cleanup" function to be run immediatley following the
        // main function. This function is intended to be quick,
        // and should execute small follow up logic, or cleanup logic.
        virtual void AttachPosteriorFunction(FunctionPtr pFunc) = 0;

    };

    // alias for shared pointer to an IExecutableWorkItem instance
    using IExecutableWorkItemPtr = std::shared_ptr<IExecutableWorkItem>;

} // end of namespace FuturesFramework

#endif // end of header guard

#pragma once

#ifndef ASYNC_IEXECUTABLEWORKITEM_HPP
#define ASYNC_IEXECUTABLEWORKITEM_HPP

// SYSTEM INCLUDES
#include <stdexcept>

// C++ PROJECT INCLUDES
#include "Async/LibraryExport.hpp"
#include "Async/IWorkItem.hpp"
// #include "Async/WorkerThread.hpp"

// project namespace
namespace Async
{
namespace Concurrency
{
    class WorkerThread;
}
    // interface to allow IWorkItems to be executable by Schedulers.
    // This interface is exported to clients.
    class ASYNC_API IExecutableWorkItem : public IWorkItem
    {
        friend class Concurrency::WorkerThread;
    private:

        virtual bool IsDone() = 0;

    protected:

        // this is the method that an IScheduler calls to run this
        // IWorkItem. The fact that this is protected is for protection.
        // We don't want clients to have access to the Execute()
        // functionality AND we want to force the client to
        // make an IScheduler (either client created or built in)
        // to be a "friend."
        virtual Types::Result_t Execute() = 0;

    public:

        virtual ~IExecutableWorkItem() = default;

        // attach a function to be executed. This function is used to
        // resolve this IExecutableWorkItem
        virtual void AttachMainFunction(FunctionPtr pFunc) = 0;

        // when the main function is done, the client can provide
        // a "cleanup" function to be run immediatley following the
        // main function. This function is intended to be quick,
        // and should execute small follow up logic, or cleanup logic.
        virtual void AttachPosteriorFunction(FunctionPtr pFunc) = 0;

        virtual std::exception_ptr GetException() const = 0;

    };

    // alias for shared pointer to an IExecutableWorkItem instance
    using IExecutableWorkItemPtr = std::shared_ptr<IExecutableWorkItem>;

} // end of namespace Async

#endif // end of header guard

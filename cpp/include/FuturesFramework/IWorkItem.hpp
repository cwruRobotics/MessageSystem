#pragma once

#ifndef FUTURESFRAMEWORK_IWORKITEM_HPP
#define FUTURESFRAMEWORK_IWORKITEM_HPP

// SYSTEM INCLUDES
#include <functional>
#include <memory>
#include <string>

// C++ PROJECT INCLUDES
#include "FuturesFramework/LibraryExport.hpp"
#include "FuturesFramework/Result.hpp"

namespace FuturesFramework
{

    // We need to declare IScheduler here. There is a class
    // in IScheduler.hpp that contains the class IScheduler.
    // However, if we "#include IScheduler.hpp" here (because)
    // we need it for the IWorkItem class, and "#include IWorkItem.hpp"
    // in class IScheduler, then we will have a circular dependency.

    // This is when the compiler cannot make sense of this file,
    // it needs to know how much space an instance of an IWorkItem takes
    // but it cannot do so without determining how much space an IScheduler
    // takes....which it needs to know how much space an IWorkItem takes
    // to do. So, if we declare IScheduler here, and then "#include IWorkItem.hpp"
    // in IScheduler.hpp, then the compiler can resolve how much space IScheduler
    // needs when defining IWorkItem.
    class IScheduler;

    // alias. Whenever the compiler sees "ISchedulerPtr" it will replace
    // it with  std::shared_ptr<IScheduler>. This saves us keystrokes.
    using ISchedulerPtr = std::shared_ptr<IScheduler>;

    // another alias.
    using WorkItemFunctionPtr = std::function<Types::Result_t()>;

    // see file LibraryExport.hpp. The tag FUTURESFRAMEWORK_API
    // is a preprocessor variable that is used to either
    // export or import class IWorkItem.
    // An IWorkItem is the base type of all executable code in FuturesFramework.
    // it represents an abstract piece of code that the client wants to execute
    // asynchronously. This is the base type that the clients can derive from,
    // and provides no other functionality than the ability for itself to be
    // executed on an IScheduler.
    class FUTURESFRAMEWORK_API IWorkItem
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

        virtual ~IWorkItem() = default;

        virtual void AttachMainFunction(WorkItemFunctionPtr pFunc) = 0;

        virtual void AttachPosteriorFunction(WorkItemFunctionPtr pFunc) = 0;

        virtual Types::Result_t Schedule(ISchedulerPtr pScheduler) = 0;

        virtual const uint64_t GetId() = 0;

    };

    using IWorkItemPtr = std::shared_ptr<IWorkItem>;

}

#endif

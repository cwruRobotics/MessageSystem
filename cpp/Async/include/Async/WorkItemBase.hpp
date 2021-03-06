#pragma once

#ifndef ASYNC_IWORKITEM_HPP
#define ASYNC_IWORKITEM_HPP

// SYSTEM INCLUDES
#include <functional>
#include <memory>
#include <string>

// C++ PROJECT INCLUDES
#include "Async/LibraryExport.hpp"
#include "Async/Result.hpp"
#include "Async/WorkItemStates.hpp"
#include "Async/JobPriorities.hpp"

namespace Async
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
    class SchedulerBase;

    // alias. Whenever the compiler sees "ISchedulerPtr" it will replace
    // it with  std::shared_ptr<IScheduler>. This saves us keystrokes.
    using SchedulerBasePtr = std::shared_ptr<SchedulerBase>;

    // another alias.
    using FunctionPtr = std::function<States::WorkItemState()>;

    // see file LibraryExport.hpp. The tag ASYNC_API
    // is a preprocessor variable that is used to either
    // export or import class IWorkItem.
    // An IWorkItem is the base type of all executable code in Async.
    // it represents an abstract piece of code that the client wants to execute
    // asynchronously. This is the base type that the clients can derive from,
    // and provides no other functionality than the ability for itself to be
    // executed on an IScheduler.
    class ASYNC_API WorkItemBase
    {
    public:

        virtual ~WorkItemBase() = default;

        virtual Types::Result_t Schedule(SchedulerBasePtr pScheduler) = 0;

        virtual void SetId(uint64_t id) = 0;

        virtual const uint64_t GetId() = 0;

        virtual const Types::JobPriority GetPriority() = 0;

    };

    using WorkItemBasePtr = std::shared_ptr<WorkItemBase>;

}

#endif

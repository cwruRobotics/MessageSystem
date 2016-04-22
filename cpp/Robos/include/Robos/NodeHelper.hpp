#pragma once
#ifndef ROBOS_NODEHELPER_HPP
#define ROBOS_NODEHELPER_HPP

// SYSTEM INCLUDES
#include <string>
#include "Async/JobPriorities.hpp"

// C++ PROJECT INCLUDES
#include "Robos/LibraryExport.hpp"

namespace Robos
{

    class ROBOS_API NodeHelper
    {
    private:

        const std::string           _name;

        // this *should* correspond to the Scheduler name (in Async) that
        // this Node should run on.
        std::string                 _executionTopic;
        Async::Types::JobPriority   _priority;

    public:

        NodeHelper(std::string name, std::string toRunOn,
                   Async::Types::JobPriority priority);

        ~NodeHelper();

        const std::string& GetName();

        std::string& GetExecutionTopic();

        Async::Types::JobPriority& GetPriority();

    };

} // end of namespace Robos

#endif // end of ROBOS_NODEHELPER_HPP

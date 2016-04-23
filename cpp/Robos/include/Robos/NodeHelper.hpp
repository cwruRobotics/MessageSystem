#pragma once
#ifndef ROBOS_NODEHELPER_HPP
#define ROBOS_NODEHELPER_HPP

// SYSTEM INCLUDES
#include <string>                   // std::string
#include "Async/JobPriorities.hpp"  // Async::Types::JobPriority

// C++ PROJECT INCLUDES
#include "Robos/LibraryExport.hpp"

/**
 * @namespace Robos. The Robos namespace
 */
namespace Robos
{

    /**
     * Internal storage for memory common to all Node instances.
     */
    class ROBOS_API NodeHelper
    {
    private:

        const std::string           _name;

        // this *should* correspond to the Scheduler name (in Async) that
        // this Node should run on.
        std::string                 _executionTopic;
        Async::Types::JobPriority   _priority;

    public:

        /**
         * A constructor.
         * @param[in]   name        The name of the Node instance.
         * @param[in]   toRunOn     The name of the Scheduler that should
                                    manage this instance.
         * @param[in]   priority    The priority of thread that this
                                    instance can be run on.
         */
        NodeHelper(std::string name, std::string toRunOn,
                   Async::Types::JobPriority priority);

        /**
         * A destructor. Declared virtual to allow inheritance.
         */
        virtual ~NodeHelper();

        /**
         * Get the name of this instance.
         * @param[out]  std::string&    The name of this instance.
         */
        const std::string& GetName();

        /**
         * Get the Scheduler name of this instance.
         * @param[out]  std::string&    The name of the Scheduler that should manage
         *                              this instance.
         */
        std::string& GetExecutionTopic();

        /**
         * Get the priority of threads that this instance can run on.
         * @param[out]  Async::Types::JobPriority&  The priority of threads that can execute
         *                                          code called by this instance.
         */
        Async::Types::JobPriority& GetPriority();

    };

} // end of namespace Robos

#endif // end of ROBOS_NODEHELPER_HPP

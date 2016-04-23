#pragma once
#ifndef ROBOS_NODEBASE_HPP
#define ROBOS_NODEBASE_HPP

// SYSTEM INCLUDES
#include <memory>   // std::shared_ptr, std::enable_shared_from_this
#include <string>   // std::string
#include <vector>   // std::vector

// C++ PROJECT INCLUDES
#include "Robos/LibraryExport.hpp"
#include "Robos/NodeHelper.hpp"
#include "Robos/MessageBase.hpp"

/**
 * @namespace Robos. The Robos namespace
 */
namespace Robos
{

    /**
     * A base class for traditional Node structures.
     * Client code is "wrapped" into a Node instance. There are
     * basically two type of Node functionality: a Node that
     * subscribes/publishes messages and a Node that only publishes
     * messages.
     * This class is for Nodes that subscribes and publish messages.
     * Nodes that derive this class can be included in arbitrary
     * placement in the execution graph including cycles.
     */
    class ROBOS_API NodeBase : public NodeHelper,
        public std::enable_shared_from_this<NodeBase>
    {
    private:

        const std::vector<std::string>  _subscriptions;

        /**
          * Client code entry point.
          * This method is used to call client code. Please
          * override this method; it acts sort of as the C++
          * side of a "main" function call into client code.
          * @param[in]  pMessage       The message to execute with.
          * @param[out] MessageBasePtr The result of execution.
          */
        virtual MessageBasePtr MainCallbackImpl(const MessageBasePtr pMessage) = 0;

    public:

        /**
         * A constructor.
         * @param[in]                   name The Name of this instance. Must be
         *                              semi-unique. There cannot be two
         *                              Nodes with the same name that
         *                              subscribe to the same Message topic.
         * @param[in]   toRunOn         The name of the Scheduler that should
         *                              manage this instance. There should
         *                              be a corresponding Scheduler entry
         *                              in the Async config file.
         * @param[in]   subscriptions   A vector of strings corresponding
         *                              to existing message topics.
         * @param[in]   priority        The priority of thread that this
         *                              instance can be run on.
         */
        NodeBase(std::string name, std::string toRunOn,
                 std::vector<std::string> subscriptions,
                 Async::Types::JobPriority priority=Async::Types::JobPriority::OTHER);

        /**
         * A destructor. Virtual so that classes can derive from it.
         */
        virtual ~NodeBase();

        /**
         * Get the vector of subscriptions that this node subscribes to.
         * @param[out]  std::vector<std::string>& The list of subscriptions.
         */
        const std::vector<std::string>& GetSubscriptions();

        /**
         * The "well known entry point" into a Node instance.
         * The only function of this method is method is to call MainCallbackImpl().
         * This is to separate customization of a method with implementation purposes.
         * @param[in]   pMessage        The message to execute with.
         * @param[out]  MessageBasePtr  The result of execution.
         */
        MessageBasePtr MainCallback(const MessageBasePtr pMessage);
    };

    /**
     * alias for a shared pointer to a NodeBase instance.
     */
    using NodeBasePtr = std::shared_ptr<NodeBase>;

}

#endif // end of ROBOS_NODEBASE_HPP

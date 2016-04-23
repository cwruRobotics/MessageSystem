#pragma once
#ifndef ROBOS_INITNODEBASE_HPP
#define ROBOS_INITNODEBASE_HPP

// SYSTEM INCLUDES
#include <memory>   // std::shared_ptr, std::enable_shared_from_this
#include <string>   // std::string

// C++ PROJECT INCLUDES
#include "Robos/LibraryExport.hpp"
#include "Robos/NodeHelper.hpp"
#include "Robos/MessageBase.hpp"

/**
 * @namespace Robos The Robos namespace
 */
namespace Robos
{

/**
 * @namespace Internal Namespace internal to Robos. Needed here to avoid circular dependency.
 */
namespace Internal
{
    /**
     * The master node held internally to Robos. This cannot be modified by clients.
     */
    class MasterNode;
} // end of namespace Internal

    /**
     * A base class for a unique Node type.
     * The second type of functionality that Nodes can have is to just produce messages
     * but have no dependencies of their own (relative to other Nodes).
     * Nodes of this type only produce messages. They should consume their own thread
     * because they usually execute continuously until the call to Robos::Stop() is made.
     */
    class ROBOS_API InitNodeBase : public NodeHelper,
        public std::enable_shared_from_this<InitNodeBase>
    {
    private:

        std::shared_ptr<Internal::MasterNode>   _pMaster;

        /**
         * The method that calls client code.
         * Clients need to override this method. This method is only called once when the call
         * to Robos::Start() is made, and it is assumed that this method will not exit until
         * Robos::Stop() is made. This may not be the case (the method can exit early if client
         * code returns), but is not called again by Robos if it exits before the call to Robos::Stop().
         */
        virtual void MainCallbackImpl() = 0;

        /**
         * Registers the MasterNode with this instance. InitNodeBase instances use a slightly
         * different message publishing technique than other Node types that requires an InitNodeBase
         * to have a pointer to the master node.
         * @param[in]   pMaster     The master node.
         */
        void RegisterMaster(std::shared_ptr<Internal::MasterNode> pMaster);

    protected:

        /**
         * Publish a message to Robos.
         * Publishing a message to Robos will cause Nodes that subscribe to the topic of the published
         * message to be executed.
         * @param[in]   pMessage    The message to pass along.
         */
        void PublishMessage(MessageBasePtr pMessage);

    public:

        /**
         * A constructor.
         * @param[in]   name    The name of this Node instance.
         * @param[in]   toRunOn The name of the Scheduler that should manage this instance.
         */
        InitNodeBase(std::string name, std::string toRunOn);

        /**
         * A destructor. Virtual to allow inheritance.
         */
        virtual ~InitNodeBase();

        /**
         * The "well known entry point" into a InitNodeBase instance. This acts as a "main"
         * call into client code.
         * @param[out]  int     0 for success throughout execution, error otherwise.
         */
        int MainCallback();
    };

    /**
     * An alias for a shared pointer to a InitNodeBase instance.
     */
    using InitNodeBasePtr = std::shared_ptr<InitNodeBase>;

}

#endif // end of ROBOS_INITNODEBASE_HPP

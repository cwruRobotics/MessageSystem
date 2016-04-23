#pragma once
#ifndef ROBOS_INTERNAL_MASTERNODE_HPP
#define ROBOS_INTERNAL_MASTERNODE_HPP

// SYSTEM INCLUDES
#include <atomic>                       // std::atomic
#include <memory>                       // std::shared_ptr
#include <vector>                       // std::vector
#include <Async/AsyncExecution.hpp>     // Async::Execute
#include <Async/Promise.hpp>            // Async::PromisePtr<...>::Then
#include <Utilities/Semaphore.hpp>      // Utilities::Semaphore instances.

// C++ PROJECT INCLUDES
#include "Robos/MessageBase.hpp"
#include "Robos/NodeDatabase.hpp"
#include "Robos/NodeBase.hpp"
#include "Robos/InitNodeBase.hpp"

/**
 * @namespace Robos the Robos namespace
 */
namespace Robos
{

/**
 * @namespace Internal Namespace internal to Robos.
 */
namespace Internal
{

    /**
     * The master node. During execution, there is a singleton instance of this class.
     * The master node manages subscriber relationships and internally keeps a pointer
     * to the graph of execution (directed graph of Nodes with an edge
     * between Nodes if and only if there is a subscriber/publisher relationship with those two Nodes).
     * The master node manages the dispatching of Nodes for execution on the Async singleton.
     */
    class MasterNode
    {
    private:

        // the database
        NodeDatabasePtr                 _pUnprotectedNodeDB;

        // the reader-writer problem reader count
        std::atomic<int>                _readCount;

        // the reader-writer problem semaphores:
        Utilities::Semaphore            _resourceAccess;    // access to shared resource
        Utilities::Semaphore            _readCountAccess;   // access to readCount
        Utilities::Semaphore            _serviceQueue;      // fairness between readers and writers

        std::atomic<bool>               _initFlag;          // true when MasterNode has started
        std::vector<InitNodeBasePtr>    _initNodes;         // the nodes that have no subscriptions

    private:

        /**
         * A method to hash a string id. This is used to compute the id
         * of a Node name in the Node database. This hash is computed as:
         *      s[0] * (31^(n - 1)) + s[1] * (31^(n - 2)) + ... + s[n - 1] for n = 1 ... string.length
         * @param[in]   topic           The string to hash.
         * @param[out]  unsigned int    The hash of the string.
         */
        unsigned int HashTopic(const std::string& topic);

    public:

        /**
         * A constructor.
         */
        MasterNode();

        /**
         * A destructor. Not virtual to prevent inheritance.
         */
        ~MasterNode();

        /**
         * Starts execution. This method returns when all InitNodes that have been registered
         * with the master singleton have been dispatched to their respective threads.
         * Note that upon this call, the master singleton will not allow other Nodes
         * to be registered for execution. This is for security and preventing attack code
         * from trying to insert malicious Nodes during execution.
         */
        void Start();

        /**
         * Stops execution. This method returns when the execution graph has been cleared.
         * If messages happen to be published after this call, they will fall on "deaf ears"
         * as the master will not relay any messages. This does not destroy the master singleton
         * or shut down the asynchronous environment.
         */
        void Stop();

        /**
         * Registers a NodeBase instance with the master singleton. If called after Start(),
         * this call has no effect. If successful, the NodeBase instance will be added to the execution
         * graph and if a Message is produced that the NodeBase instance subscribes to,
         * the NodeBase will be executed on its specified Scheduler and thread (assuming they exist).
         * @param[in]   pNodeBase   shared pointer to the Node instance to be registered.
         * @param[out]  bool        true if pNodeBase was registered successfully, false otherwise.
         */
        bool Register(const NodeBasePtr pNodeBase);

        /**
         * Registers an InitNodeBase instance with the master singleton. This method works
         * exactly the same as Register(const NodeBasePtr pNodeBase). See documentation above.
         * @param[in]   pInitNodeBase   shared pointer to the InitNode instance to be registered.
         * @param[out]  bool            true if pInitNodeBase was registered successfully, false otherwise.
         */
        bool RegisterInitNode(const InitNodeBasePtr pInitNodeBase);

        /**
         * Dispatches all subscribers to a message. Given a message, the execution graph is read
         * and all Nodes who subscribe to the message's topic are dispatched on their respective
         * threads.
         * @param[in]   pMessage    The message passed to children.
         */
        void InvokeSubscribers(const MessageBasePtr pMessage);

    };

    /**
     * An alias for a shared pointer to a MasterNode instance.
     */
    using MasterNodePtr = std::shared_ptr<MasterNode>;


} // end of namespace Internal

    /**
     * An internal function to publish a message to MasterNode. This is half of the interface
     * for how InitNodes publish messages. This function is not exported to clients except
     * indirectly through the InitNodeBase class.
     * @param[in]   pMessage    The message to publish.
     */
    void Publish(MessageBasePtr pMessage);

} // end of namespace Robos


#endif // end of ROBOS_INTERNAL_MASTERNODE_HPP

#ifndef ROBOS_INTERNAL_MASTERNODE_HPP
#define ROBOS_INTERNAL_MASTERNODE_HPP

// SYSTEM INCLUDES
#include <atomic>   // std::atomic
#include <memory>   // std::shared_ptr
#include <vector>
#include <Async/AsyncExecution.hpp>     // Async::Execute
#include <Async/Promise.hpp>            // Async::PromisePtr<...>::Then
#include <Utilities/Semaphore.hpp>

// C++ PROJECT INCLUDES
#include "Robos/MessageBase.hpp"
#include "Robos/NodeDatabase.hpp"
#include "Robos/NodeBase.hpp"
#include "Robos/InitNodeBase.hpp"

namespace Robos
{
namespace Internal
{

    // giving a name for std::shared_ptr<Async::Promise<std::shared_ptr<Robos::MessageBase> > >
    // using ServiceRequestPtr = Async::PromisePtr<MessageBasePtr>;

    class MasterNode
    {
    private:

        // the database
        NodeDatabasePtr                 _pUnprotectedNodeDB;

        // the reader-writer problem reader count
        std::atomic<int>                _readCount;
        // the reader-writer problem semaphores
        Utilities::Semaphore            _resourceAccess;    // access to shared resource
        Utilities::Semaphore            _readCountAccess;   // access to readCount
        Utilities::Semaphore            _serviceQueue;      // fairness between readers and writers

        std::atomic<bool>               _initFlag;          // true when MasterNode has started
        std::vector<InitNodeBasePtr>    _initNodes;         // the nodes that have no subscriptions

    private:

        unsigned int HashTopic(const std::string& topic);

    public:

        MasterNode();

        ~MasterNode();

        void Start();

        void Stop();

        bool Register(const NodeBasePtr pNodeBase);

        bool RegisterInitNode(const InitNodeBasePtr pInitNodeBase);

        void InvokeSubscribers(const MessageBasePtr pMessage);

    };

    using MasterNodePtr = std::shared_ptr<MasterNode>;


} // end of namespace Internal

    void Publish(MessageBasePtr pMessage);

} // end of namespace Robos


#endif // end of ROBOS_INTERNAL_MASTERNODE_HPP

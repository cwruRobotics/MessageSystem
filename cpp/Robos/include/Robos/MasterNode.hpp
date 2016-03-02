#ifndef ROBOS_INTERNAL_MASTERNODE_HPP
#define ROBOS_INTERNAL_MASTERNODE_HPP

// SYSTEM INCLUDES
#include <atomic>   // std::atomic
#include <memory>   // std::shared_ptr
#include <Async/AsyncExecution.hpp>     // Async::Execute
#include <Async/Promise.hpp>            // Async::PromisePtr<...>::Then

// C++ PROJECT INCLUDES
#include "Utilities/Semaphore.hpp"
#include "Robos/MessageBase.hpp"
#include "Robos/NodeBase.hpp"
#include "Robos/NodeDatabase.hpp"

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
        NodeDatabasePtr         _pUnprotectedNodeDB;

        // the reader-writer problem reader count
        std::atomic<int>        _readCount;
        // the reader-writer problem semaphores
        Utilities::Semaphore    _resourceAccess;    // access to shared resource
        Utilities::Semaphore    _readCountAccess;   // access to readCount
        Utilities::Semaphore    _serviceQueue;      // fairness between readers and writers

    private:

        unsigned int HashTopic(const std::string& topic);

    public:

        MasterNode();

        ~MasterNode();

        bool Register(const NodeBasePtr& pNodeBase);

        void InvokeSubscribers(const MessageBasePtr& pMessage);

        bool Shutdown();
    };

    using MasterNodePtr = std::shared_ptr<MasterNode>;

} // end of namespace Internal
} // end of namespace Robos


#endif // end of ROBOS_INTERNAL_MASTERNODE_HPP

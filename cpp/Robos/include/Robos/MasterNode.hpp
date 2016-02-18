#ifndef ROBOS_INTERNAL_MASTERNODE_HPP
#define ROBOS_INTERNAL_MASTERNODE_HPP

// SYSTEM INCLUDES
#include <atomic>   // std::atomic

// C++ PROJECT INCLUDES
#include "Utilities/Semaphore.hpp"

#include "Async/AsyncExecution.hpp"
#include "Async/Promise.hpp"

#include "Robos/MessageBase.hpp"
#include "Robos/NodeBase.hpp"
#include "Robos/NodeDatabase.hpp"

namespace Robos
{
namespace Internal
{

    // giving a name for std::shared_ptr<Async::Promise<std::shared_ptr<Robos::MessageBase> > >
    using ServiceRequestPtr = Async::PromisePtr<MessageBasePtr>;

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

    public:

        MasterNode();

        ~MasterNode();

        bool Shutdown();
    };

} // end of namespace Internal
} // end of namespace Robos


#endif // end of ROBOS_INTERNAL_MASTERNODE_HPP

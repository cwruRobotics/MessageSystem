// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/MasterNode.hpp"

namespace Robos
{
namespace Internal
{

    MasterNode::MasterNode() : _pUnprotectedNodeDB(), _readCount(0),
        _resourceAccess(1), _readCountAccess(1), _serviceQueue(1)
    {
    }

    MasterNode::~MasterNode()
    {
    }

    bool MasterNode::Shutdown()
    {
        return false;
    }

} // end of namespace Internal
} // end of namespace Robos

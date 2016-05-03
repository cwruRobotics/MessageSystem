// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/NodeHelper.hpp"
#include "Robos/Robos.hpp"

namespace Robos
{

    NodeHelper::NodeHelper(std::string name, std::string toRunOn,
                           Async::Types::JobPriority priority) :
        _name(name), _executionTopic(toRunOn), _priority(priority)
    {
        
    }

    NodeHelper::~NodeHelper()
    {
    }

    const std::string& NodeHelper::GetName()
    {
        return this->_name;
    }

    std::string& NodeHelper::GetExecutionTopic()
    {
        return this->_executionTopic;
    }

    Async::Types::JobPriority& NodeHelper::GetPriority()
    {
        return this->_priority;
    }

    void NodeHelper::ShutdownRobos()
    {
        ShutdownRobosFromNode();
    }

} // end of namespace Robos

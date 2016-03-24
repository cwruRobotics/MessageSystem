
// SYSTEM INCLUDES
#include <stdexcept>

// C++ PROJECT INCLUDES
#include "Async/JobPriorities.hpp"

namespace Async
{

    Types::JobPriority GetPriorityFromString(const std::string priority)
    {
        if(priority == "IMMEDIATE")
        {
            return Types::JobPriority::IMMEDIATE;
        }
        else if(priority == "RELAXED")
        {
            return Types::JobPriority::RELAXED;
        }
        else if(priority == "OTHER")
        {
            return Types::JobPriority::OTHER;
        }
        throw std::logic_error("Unknown job priority" + priority);
    }

} // end of namespace Async

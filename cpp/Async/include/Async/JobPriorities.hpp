
#ifndef ASYNC_TYPES_JOBPRIORITIES_HPP
#define ASYNC_TYPES_JOBPRIORITIES_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES


namespace Async
{
namespace Types
{
    enum class JobPriority
    {
        IMMEDIATE = 0,
        RELAXED = 1,
        OTHER = 2,
    };
} // end of namespace Types

Types::JobPriority GetPriorityFromString(const std::string priority);

} // end of namespace Async

#endif // end of ASYNC_TYPES_JOBPRIORITIES_HPP

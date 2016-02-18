#ifndef STRUCTURES_TEST_COMMONMEMORY_HPP
#define STRUCTURES_TEST_COMMONMEMORY_HPP

// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES
#include "Utilities/Semaphore.hpp"

namespace Utilities
{
namespace Tests
{

    struct CommonMemory
    {
        Semaphore   _semaphore;

        CommonMemory(int semInit);

        ~CommonMemory();
    };

    using CommonMemoryPtr = std::shared_ptr<CommonMemory>;

} // end of namespace Tests
} // end of namespace Utilities

#endif // end of STRUCTURES_TEST_COMMONMEMORY_HPP

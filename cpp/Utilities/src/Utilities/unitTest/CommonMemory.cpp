// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Utilities/unitTest/CommonMemory.hpp"

namespace Utilities
{
namespace Tests
{

    CommonMemory::CommonMemory(int semInit) : _semaphore(semInit)
    {
    }

    CommonMemory::~CommonMemory()
    {
    }

} // end of namespace Tests
} // end of namespace Utilities

// SYSTEM INCLUDES
#include <iostream>

// C++ PROJECT INCLUDES
#include "Async/unitTest/TestUtilities.hpp"

namespace Async
{
namespace Tests
{
namespace Utilities
{

    void WriteUnitTestNameToConsole(std::string unitTestName)
    {
        std::cout << "Running[" << unitTestName.c_str() << "]" << std::endl;
    }

}
}
}
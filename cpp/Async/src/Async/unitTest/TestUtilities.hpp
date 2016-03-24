#ifndef ASYNC_UNITTEST_TESTUTILITES_HPP
#define ASYNC_UNITTEST_TESTUTILITES_HPP

// SYSTEM INCLUDES
#include <vector>
#include <Utilities/rapidxml.hpp>
#include <Utilities/rapidxml_utils.hpp>

// C++ PROJECT INCLUDES
#include "Async/JobPriorities.hpp"

namespace Async
{
namespace Tests
{
namespace Utilities
{

    void WriteUnitTestNameToConsole(std::string unitTestName);

    std::vector<Types::JobPriority> GetDefaultSchedulerConfig();

    std::string GetCurrentDirectory(std::string pathToFile);

    std::vector<std::string> GetSchedulersToCheckFor(std::string configFilePath);

} // end of namespace Utilities
} // end of namespace Tests
} // end of namespace Async

#endif // end of ASYNC_UNITTEST_TESTUTILITES_HPP

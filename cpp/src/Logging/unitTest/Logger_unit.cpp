
// SYSTEM INCLUDES
#include <string>

// C++ PROJECT INCLUDES
#include "Logging/unitTest/catchPort.hpp"
#include "Logging/ILogger.hpp"
#include "Logging/Factory.hpp"

namespace Logging
{
namespace Tests
{

    TEST_CASE("GetName should reflect logger name set in constructor", "[Logger_unit]")
    {
        const char* testLoggerName = "My.Test.Name";
        LoggerPtr testLogger = Factory::makeLogger(testLoggerName);

        REQUIRE( strcmp(testLoggerName, testLogger->GetName()) == 0 );
    }

    TEST_CASE("GetOutputFile should reflect output file set in constructor", "[Logger_unit]")
    {
        const char* testLoggerName = "My.Test.Name";
        const char* testOutputFile = "C:/MyOutputFile/test.txt";

        // test providing an output file to write to
        LoggerPtr testLogger = Factory::makeLogger(testLoggerName, testOutputFile);

        REQUIRE( strcmp(testOutputFile, testLogger->GetOutputFile()) == 0 );

        // test writing to standard out if no output file is provided
        testLogger = Factory::makeLogger(testLoggerName);
        
        REQUIRE( strcmp(testLogger->GetOutputFile(), "std::cout") == 0 );
    }
}
}

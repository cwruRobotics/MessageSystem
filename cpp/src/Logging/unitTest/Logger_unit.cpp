
// SYSTEM INCLUDES
#include <fstream>
#include <string>

// C++ PROJECT INCLUDES
#include "Logging/unitTest/catchPort.hpp"
#include "Logging/unitTest/LoggerTestHelper.hpp"
#include "Logging/ILogger.hpp"
#include "Logging/Factory.hpp"

namespace Logging
{
namespace Tests
{

    TEST_CASE("GetName should reflect logger name set in constructor", "[Logger_unit]")
    {
        std::string testLoggerName = "My.Test.Name";
        LoggerPtr testLogger = Factory::makeLogger(testLoggerName->c_str());

        std::string setName = testLogger->GetName();
        REQUIRE( testLoggerName == setName );
    }

    TEST_CASE("GetOutputFile should reflect output file set in constructor", "[Logger_unit]")
    {
        std::string testLoggerName = "My.Test.Name";
        std::string testOutputFile = "C:/MyOutputFile/test.txt";
        std::string unspecifiedOutputFile = "std::cout";

        // test providing an output file to write to
        LoggerPtr testLogger = Factory::makeLogger(testLoggerName, testOutputFile);

        std::string setOutputFile = testLogger->GetOutputFile();
        REQUIRE( testOutputFile == setOutputFile );

        // test writing to standard out if no output file is provided
        testLogger = Factory::makeLogger(testLoggerName);
        setOutputFile = testLogger->GetOutputFile();
        
        REQUIRE( setOutputFile == unspecifiedOutputFile );
    }

    /**
    TEST_CASE("Logging has a specific signature", "[Logger_unit]")
    {
        std::string testLoggerName = "My.Test.Name";
        std::string expectedMessage = "[] test message!";

        LoggingTestHelper() helper;
        std::string testOutputFile = helper.GetCurrentDirectory(__FILE__, "testLog.txt");
        LoggerPtr testLogger = Factory::makeLogger(testLoggerName, testOutputFile.c_str());
        LOG_INFO(testLogger, "test message!");
        std::ifstream testFile(testOutputFile.c_str());
        std::string testFileContents;
        testFileContents.assign((std::istreambuf_iterator<char>(testFile)),
                                (std::istreambuf_iterator<char>()));

        REQUIRE( expectedMessage == testFileContents );

        testFile.close();
    }
    */
}
}

// int main(int argc, char* argv[])
// {
//     testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }

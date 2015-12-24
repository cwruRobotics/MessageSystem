
// SYSTEM INCLUDES
#include <fstream>
#include <string>

// C++ PROJECT INCLUDES
#include "gtest/gtest.h"
#include "Logging/ILogger.hpp"
#include "Logging/Factory.hpp"

namespace Logging
{
namespace Tests
{
    class LoggingTests : public ::testing::Test
    {
    public:
        virtual void SetUp() override
        {
        }
        
        virtual void TearDown()
        {
        }

        std::string getCurrentDirectory(const char* const pCurrentFile, std::string fileName)
        {
            // travel backwards through the string and find the first occurance
            // of a '/'. Return the substring.
            std::string wrappedCurrentFile = pCurrentFile;
            int indexOfLastDir = -1;
            for(int index = wrappedCurrentFile.length() - 1; index > -1 && indexOfLastDir == -1; --index)
            {
                if(wrappedCurrentFile[index] == '/' || wrappedCurrentFile[index] == '\'')
                {
                    indexOfLastDir = index;
                }
            }
            if(indexOfLastDir != -1)
            {
                return wrappedCurrentFile.substr(0, indexOfLastDir) + fileName; 
            }
            return "";
        }

    };

    TEST_F(LoggingTests, TestConstructorAndGetName)
    {
        const char* testLoggerName = "My.Test.Name";
        LoggerPtr testLogger = Factory::makeLogger(testLoggerName);
        EXPECT_EQ(strcmp(testLogger->GetName(), testLoggerName), 0)
            << "Logger has wrong name, expected: " << testLoggerName
            << " but was: " << testLogger->GetName();
    }

    TEST_F(LoggingTests, TestConstructorAndGetOutputFile)
    {
        const char* testLoggerName = "My.Test.Name";
        const char* testOutputFile = "C:/MyOutputFile/test.txt";

        // test providing an output file to write to
        LoggerPtr testLogger = Factory::makeLogger(testLoggerName, testOutputFile);
        EXPECT_EQ(strcmp(testLogger->GetOutputFile(), testOutputFile), 0)
            << "Logger has wrong output file, expected: " << testOutputFile
            << " but was: " << testLogger->GetOutputFile();

        // test writing to standard out if no output file is provided
        testLogger = Factory::makeLogger(testLoggerName);
        EXPECT_EQ(strcmp(testLogger->GetOutputFile(), "std::cout"), 0)
            << "Logger has wrong output file, expected: std::cout"
            << " but was: " << testLogger->GetOutputFile();
    }

    TEST_F(LoggingTests, TestLoggingToFile)
    {
        const char* testLoggerName = "My.Test.Name";
        std::string testOutputFile = this->getCurrentDirectory(__FILE__, "testLog.txt");
        LoggerPtr testLogger = Factory::makeLogger(testLoggerName, testOutputFile.c_str());
        LOG_INFO(testLogger, "test message!");
        std::ifstream testFile(testOutputFile.c_str());
        std::string testFileContents;
        testFileContents.assign((std::istreambuf_iterator<char>(testFile)),
                                (std::istreambuf_iterator<char>()));

        EXPECT_EQ(testFileContents.compare("test message!"), 0)
            << "Logger has not logged correctly, expected: test message!"
            << " but was: " << testFileContents.c_str();

        testFile.close();
    }
}
}

// int main(int argc, char* argv[])
// {
//     testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }

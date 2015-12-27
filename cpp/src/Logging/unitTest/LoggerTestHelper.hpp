#pragma once

#ifndef LOGGING_UNITTESTS_LOGGERTESTHELPER_HPP
#define LOGGING_UNITTESTS_LOGGERTESTHELPER_HPP

// SYSTEM INCLUDES
#include <string>

// C++ PROJECT INCLUDES

namespace Logging
{
namespace Tests
{
    class LoggerTestHelper
    {
    public:
        LoggerTestHelper()
        {
        }

        ~LoggerTestHelper()
        {
        }

        std::string GetCurrentDirectory(const char* const pCurrentFile, std::string fileName);
    };
}
}

#endif

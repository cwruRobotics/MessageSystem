#pragma once

#ifndef LOGGING_MESSAGES_LOGGINGLEVELS_HPP
#define LOGGING_MESSAGES_LOGGINGLEVELS_HPP

// SYSTEM INCLUDES
#include <string>

// C++ PROJECT INCLUDES
#include "Logging/LibraryExport.hpp"

// project namespace
namespace Logging
{
// component namespace
namespace Messages
{

    // this class represents the "type" of log message. The type of message
    // represents what kind of log message is being written..is it a
    // debug message, an info message, etc. These are not exported.
	enum class LoggingLevels
	{
		DEBUG_ = 0,     // represents a debug message
		INFO_ = 1,      // represents an info message
		WARNING_ = 2,   // represents a warning message
		ERROR_ = 3,     // represents an error message
	};

} // end of namespace Messages

// a method to convert a logging message to a string.
std::string LoggingLevelToString(const Messages::LoggingLevels& level);

} // end of namespace Logging

#endif // end of header guard

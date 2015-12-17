#pragma once

#ifndef LOGGING_MESSAGES_LOGGINGLEVELS_HPP
#define LOGGING_MESSAGES_LOGGINGLEVELS_HPP

// SYSTEM INCLUDES
#include <string>

// C++ PROJECT INCLUDES
#include "Logging/SharedLibExport.hpp"

namespace Logging
{
namespace Messages
{

	enum class LoggingLevels
	{
		DEBUG = 0,
		INFO = 1,
		WARNING = 2,
		ERROR = 3,
	};

}
std::string LoggingLevelToString(const Messages::LoggingLevels& level);
}

#endif


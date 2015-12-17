// SYSTEM INCLUDES

// C++ PROJECT INCLUDES
#include "Logging/LoggingLevels.hpp"

namespace Logging
{

	std::string LoggingLevelToString(const Messages::LoggingLevels& level)
	{
		switch (level)
		{
		case Messages::LoggingLevels::DEBUG:
			return "DEBUG";
		case Messages::LoggingLevels::INFO:
			return "INFO";
		case Messages::LoggingLevels::WARNING:
			return "WARNING";
		case Messages::LoggingLevels::ERROR:
			return "ERROR";
		}
		throw std::exception("unknown logging level");
	}

}
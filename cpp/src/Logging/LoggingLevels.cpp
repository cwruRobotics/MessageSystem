// SYSTEM INCLUDES

// C++ PROJECT INCLUDES
#include "Logging/LoggingLevels.hpp"

namespace Logging
{

	std::string LoggingLevelToString(const Messages::LoggingLevels& level)
	{
		switch (level)
		{
		case Messages::LoggingLevels::DEBUG_:
			return "DEBUG";
		case Messages::LoggingLevels::INFO_:
			return "INFO";
		case Messages::LoggingLevels::WARNING_:
			return "WARNING";
		case Messages::LoggingLevels::ERROR_:
			return "ERROR";
		}
		throw std::logic_error("unknown logging level");
	}

}

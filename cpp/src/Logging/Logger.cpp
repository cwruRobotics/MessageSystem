// SYSTEM INCLUDES
#include <cstdarg>

// C++ PROJECT INCLUDES
#include "Logging/Logger.hpp"

namespace Logging
{
namespace Loggers
{

	const int Logger::GetMaxMessageLength()
	{
		return LOG_MESSAGE_SIZE;
	}

	const char* Logger::GetName()
	{
		return this->_loggerName;
	}

	const char* Logger::GetOutputFile()
	{
		return this->_outputFile ? this->_outputFile : "std::cout";
	}

	void Logger::Log(Messages::LoggingLevels level, const char* pFormat, ...)
	{
		va_list args;
		va_start(args, pFormat);

		char localBuffer[LOG_MESSAGE_SIZE];
		int result = vsnprintf(localBuffer, LOG_MESSAGE_SIZE, pFormat, args);
		if (this->_outputFile && this->_fileBuffer.is_open())
		{
			this->_fileBuffer << LoggingLevelToString(level) << " -> "
				<< localBuffer;
		}
		else
		{
			std::cout << LoggingLevelToString(level) << " -> "
				<< localBuffer;
		}
		va_end(args);
	}

	void Logger::Log(Messages::LoggingLevels level, const char* const pFileName,
					 int lineNumber, const char* pFormat, ...)
	{
		va_list args;
		va_start(args, pFormat);

		char localBuffer[LOG_MESSAGE_SIZE];
		int result = vsnprintf(localBuffer, LOG_MESSAGE_SIZE, pFormat, args);
		if (this->_outputFile && this->_fileBuffer.is_open())
		{
			this->_fileBuffer << "[" << this->_loggerName << " in file: " << pFileName
				<< " at line: " << lineNumber << "]" << LoggingLevelToString(level)
				<< localBuffer << std::endl;
		}
		else
		{
			std::cout << "[" << this->_loggerName << " in file: " << pFileName
				<< " at line: " << lineNumber << "]" << LoggingLevelToString(level)
				<< localBuffer << std::endl;
		}
		va_end(args);
	}

}
}

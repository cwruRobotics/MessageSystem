#pragma once

#ifndef LOGGING_LOGGERS_LOGGER_HPP
#define LOGGING_LOGGERS_LOGGER_HPP

// SYSTEM INCLUDES
#include <fstream>
#include <iostream>

// C++ PROJECT INCLUDES
#include "Logging/ILogger.hpp"

namespace Logging
{
namespace Loggers
{

#ifndef LOG_MESSAGE_SIZE
#define LOG_MESSAGE_SIZE 40
#endif

	class Logger : public ILogger
	{
	private:

		const char*		_loggerName;
		const char*		_outputFile;
		std::ofstream	_fileBuffer;

	public:

		Logger(const char* name, const char* outputFile = nullptr) :
			_loggerName(name), _outputFile(outputFile)
		{
			if (this->_outputFile)
			{
				this->_fileBuffer.open(this->_outputFile);
			}
		}

		virtual ~Logger()
		{
			if (this->_outputFile && this->_fileBuffer.is_open())
			{
				this->_fileBuffer.close();
			}
			this->_outputFile = nullptr;
			this->_loggerName = nullptr;
		}

		const int GetMaxMessageLength() override;

		const char* GetName() override;

		const char* GetOutputFile() override;

		void Log(Messages::LoggingLevels level, const char* pFormat, ...) override;

		void Log(Messages::LoggingLevels level, const char* const pFileName,
				 int lineNumber, const char* pFormat, ...) override;

	};

}
}

#endif

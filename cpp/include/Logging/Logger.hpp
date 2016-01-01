#pragma once

#ifndef LOGGING_LOGGERS_LOGGER_HPP
#define LOGGING_LOGGERS_LOGGER_HPP

// SYSTEM INCLUDES
#include <fstream>
#include <iostream>

// C++ PROJECT INCLUDES
#include "Logging/ILogger.hpp"

// project namespace
namespace Logging
{
// component namespace. This is internal to the Logging project.
namespace Loggers
{

// the max message size that loggers can write.
#ifndef LOG_MESSAGE_SIZE
#define LOG_MESSAGE_SIZE 180
#endif

    // this is the actual Logger instance, it is not exported.
	class Logger : public ILogger
	{
	private:

		const char*		_loggerName;
		const char*		_outputFile;
		std::ofstream	_fileBuffer; // buffer to the output file if it exists.

	public:

        // standard constructor. If the outputFile is not provided,
        // the logger will write to the console (std::cout).
		Logger(const char* name, const char* outputFile=nullptr) :
			_loggerName(name), _outputFile(outputFile)
		{
            // open file buffer if there is a file to write to.
            // this currently does no filePath checking.
			if (this->_outputFile)
			{
				this->_fileBuffer.open(this->_outputFile);
			}
		}

        // default destructor.
		virtual ~Logger()
		{
            // close the connection if it is open.
			if (this->_outputFile && this->_fileBuffer.is_open())
			{
				this->_fileBuffer.close();
			}
			this->_outputFile = nullptr;
			this->_loggerName = nullptr;
		}

        // get the max number of characters that this logger can log
        // in a single message.
		const int GetMaxMessageLength() override;

        // get the name of this logger.
		const char* GetName() override;

        // get the file this logger will log to. If no file was provided,
        // this will return "std::cout."
		const char* GetOutputFile() override;

        //
		void Log(Messages::LoggingLevels level, const char* pFormat, ...) override;

        //
		void Log(Messages::LoggingLevels level, const char* const pFileName,
				 int lineNumber, const char* pFormat, ...) override;

	};

} // end of namespace Loggers
} // end of namespace Logging

#endif // end of header guard

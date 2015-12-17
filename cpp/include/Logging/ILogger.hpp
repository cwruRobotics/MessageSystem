#pragma once

#ifndef LOGGING_LOGGERS_ILOGGER_HPP
#define LOGGING_LOGGERS_ILOGGER_HPP

// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES
#include "Logging/LoggingLevels.hpp"
#include "Logging/SharedLibExport.hpp"

// MACROS
#define LOG_DEBUG(pLogger, pFormat, ...) \
	(pLogger)->Log(Logging::Messages::LoggingLevels::DEBUG, \
	__FILE__, __LINE__, (pFormat), ##__VA_ARGS__)

#define LOG_INFO(pLogger, pFormat, ...) \
	(pLogger)->Log(Logging::Messages::LoggingLevels::INFO, \
	__FILE__, __LINE__, (pFormat), ##__VA_ARGS__)

#define LOG_WARNING(pLogger, pFormat, ...) \
	(pLogger)->Log(Logging::Messages::LoggingLevels::WARNING, \
	__FILE__, __LINE__, (pFormat), ##__VA_ARGS__)

#define LOG_ERROR(pLogger, pFormat, ...) \
	(pLogger)->Log(Logging::Messages::LoggingLevels::ERROR, \
	__FILE__, __LINE__, (pFormat), ##__VA_ARGS__)

namespace Logging
{
namespace Loggers
{

	class LOGGING_API ILogger
	{
	public:

		virtual ~ILogger() = default;

		virtual const int GetMaxMessageLength() = 0;

		virtual const char* GetName() = 0;

		virtual const char* GetOutputFile() = 0;

		virtual void Log(Messages::LoggingLevels level, const char* pFormat, ...) = 0;

		virtual void Log(Messages::LoggingLevels level, const char* const pFileName,
						 int lineNumber, const char* pFormat, ...) = 0;
	};

    using LoggerPtr = std::unique_ptr<ILogger>;
}
}

#endif


#pragma once

#ifndef LOGGING_LOGGERS_ILOGGER_HPP
#define LOGGING_LOGGERS_ILOGGER_HPP

// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES
#include "Logging/LoggingLevels.hpp"
#include "Logging/LibraryExport.hpp"


// MACROS
// these are how clients are intended to use
// Logger instances.
#define LOG_DEBUG(pLogger, pFormat, ...) \
	(pLogger)->Log(Logging::Messages::LoggingLevels::DEBUG_, \
	__FILE__, __LINE__, (pFormat), ##__VA_ARGS__)

#define LOG_INFO(pLogger, pFormat, ...) \
	(pLogger)->Log(Logging::Messages::LoggingLevels::INFO_, \
	__FILE__, __LINE__, (pFormat), ##__VA_ARGS__)

#define LOG_WARNING(pLogger, pFormat, ...) \
	(pLogger)->Log(Logging::Messages::LoggingLevels::WARNING_, \
	__FILE__, __LINE__, (pFormat), ##__VA_ARGS__)

#define LOG_ERROR(pLogger, pFormat, ...) \
	(pLogger)->Log(Logging::Messages::LoggingLevels::ERROR_, \
	__FILE__, __LINE__, (pFormat), ##__VA_ARGS__)

// project namespace
namespace Logging
{
// component namespace. This namespace
// is generally internal to the Logging project
// but is used to compartamentalize code.
namespace Loggers
{

    // this class is exported when this project is built, and imported
    // when included by another project. LOGGING_API is defined
    // in LibraryExport.hpp. This is the interface that all logger
    // classes must derive from.
	class LOGGING_API ILogger
	{
	public:

        // default destructor
		virtual ~ILogger() = default;

        // get the number of characters that this logger can log.
		virtual const int GetMaxMessageLength() = 0;

        // get the name of this logger.
		virtual const char* GetName() = 0;

        // get where this logger logs messages to. It is either
        // an absolute path or "std::cout" if none was provided.
		virtual const char* GetOutputFile() = 0;

        // log method. This logs the "type" of the message (the "level"),
        // and a formatted message.
		virtual void Log(Messages::LoggingLevels level, const char* pFormat, ...) = 0;

        // log method. This logs the "type" of the message as well as the file
        // in which the message was written, the line number, and a formatted message.
        // This method can be used to mimic the functionality of a debugger without
        // the ability to step in and out of the instruction stream.
		virtual void Log(Messages::LoggingLevels level, const char* const pFileName,
						 int lineNumber, const char* pFormat, ...) = 0;
	};

} // end of namespace Loggers

// alias. This is outside of the component namespace so that clients have no knowledge
// of the component namespace.
using LoggerPtr = std::shared_ptr<Loggers::ILogger>;

} // end of namespace Logging

#endif // end of header guard

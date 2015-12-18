#pragma once

#ifndef LOGGING_FACTORY_HPP
#define LOGGING_FACTORY_HPP

// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES
#include "Logging/SharedLibExport.hpp"
#include "Logging/ILogger.hpp"


namespace Logging
{
namespace Factory
{

    LoggerPtr LOGGER_API makeLogger(const char* name,
                                    const char* outputFile=nullptr);

}
}

#endif

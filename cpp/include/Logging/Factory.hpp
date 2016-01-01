#pragma once

#ifndef LOGGING_FACTORY_HPP
#define LOGGING_FACTORY_HPP

// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES
#include "Logging/LibraryExport.hpp"
#include "Logging/ILogger.hpp"


// project namespace
namespace Logging
{
// component namespace. This requires clients to call Factory::makeLogger(...)
// and reminds them that this is a factory function.
namespace Factory
{

    // this is the only way to create a Logger instance. The only classes
    // exposed to the client are ILogger and this file. This ensures
    // that the clients HAVE to use the logging macros defined in
    // ILogger.hpp
    // Note proper documentation tags will come with the arrival of doxygen
    // (our documentation software).
    LoggerPtr LOGGING_API makeLogger(const char* name,
                                     const char* outputFile=nullptr);

} // end of namespace Factory
} // end of namespace Logging

#endif // end of header guard

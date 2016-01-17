// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Logging/Logger.hpp"

namespace Logging
{
namespace Factory
{
    LoggerPtr makeLogger(const char* name,
                         const char* outputFile=nullptr)
    {
        return std::make_shared<Loggers::Logger>(name, outputFile);
    }
}
}

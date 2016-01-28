// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Logging/Factory.hpp"
#include "Logging/Logger.hpp"

namespace Logging
{
namespace Factory
{
    LoggerPtr MakeLogger(const char* name,
                         const char* outputFile)
    {
        return std::make_shared<Loggers::Logger>(name, outputFile);
    }
}
}

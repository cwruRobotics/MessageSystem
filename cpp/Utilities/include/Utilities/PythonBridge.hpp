#pragma once
#ifndef UTILITIES_EMBEDDED_BRIDGES_PYTHONBRIDGE_HPP
#define UTILITIES_EMBEDDED_BRIDGES_PYTHONBRIDGE_HPP

// SYSTEM INCLUDES
#include <vector>

// C++ PROJECT INCLUDES
#include "Utilities/PythonInclude.hpp"
#include "Utilities/LibraryExport.hpp"


namespace Embedded
{
namespace Bridges
{

    UTILITIES_API bool StartPython(std::vector<std::string> pathsToAdd);

    UTILITIES_API bool StopPython();

} // end of namespace Bridges
} // end of namespace Embedded

#endif // end of UTILITIES_EMBEDDED_BRIDGES_PYTHONBRIDGE_HPP

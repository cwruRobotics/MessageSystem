#pragma once
#ifndef UTILITIES_EMBEDDED_BRIDGES_PYTHONBRIDGE_HPP
#define UTILITIES_EMBEDDED_BRIDGES_PYTHONBRIDGE_HPP

// DO NOT TOUCH THIS LINE. PYTHON DEFINES SOME SYSTEM SPECIFIC VARIABLES ON UNIX.
// THAT GCC GENERATES WARNINGS FOR (THEY HAVE NO EFFECT ON PERFORMANCE), WHICH
// WILL FAIL THE BUILD. BY PUTTING THIS INCLUDE BEFORE SYSTEM INCLUDES WE CAN
// PREVENT THE WARNINGS.
#include "Utilities/PythonInclude.hpp"

// SYSTEM INCLUDES
#include <string>
#include <vector>

// C++ PROJECT INCLUDES
#include "Utilities/LibraryExport.hpp"


namespace Embedded
{
namespace Bridges
{

    UTILITIES_API bool StartPython(std::vector<std::string> pathsToAdd);

    UTILITIES_API bool StopPython();

    UTILITIES_API PyObject* RunPythonFile(std::string fileName, PyObject* pArgs);

} // end of namespace Bridges
} // end of namespace Embedded

#endif // end of UTILITIES_EMBEDDED_BRIDGES_PYTHONBRIDGE_HPP

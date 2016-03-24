#pragma once

#ifndef UTILITIES_OSUTILS_HPP
#define UTILITIES_OSUTILS_HPP

// SYSTEM INCLUDES
#include <string>


// C++ PROJECT INCLUDES
#include "Utilities/LibraryExport.hpp"

namespace Utilities
{
namespace OS
{

    UTILITIES_API std::string GetCurrentDirectory(std::string pathToFile);

} // end of namespace OS
} // end of namespace Utilities

#endif // end of UTILITIES_OSUTILS_HPP

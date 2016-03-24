// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Utilities/OSUtils.hpp"

namespace Utilities
{
namespace OS
{

    std::string GetCurrentDirectory(std::string pathToFile)
    {
        int indexOfLastPath = -1;
        for(int i = pathToFile.length() - 1; i > -1 && indexOfLastPath == -1; --i)
        {
            if (pathToFile.at(i) == '\\' || pathToFile.at(i) == '/')
            {
                indexOfLastPath = i;
            }
        }
        return pathToFile.substr(0, indexOfLastPath);
    }

} // end of namespace OS
} // end of namespace Utilities

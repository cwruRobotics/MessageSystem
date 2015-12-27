// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Logging/unitTest/LoggerTestHelper.hpp"

namespace Logging
{
namespace Tests
{

std::string LoggerTestHelper::GetCurrentDirectory(const char* const pCurrentFile, std::string fileName)
{
    // travel backwards through the string and find the first occurance
    // of a '/'. Return the substring.
    std::string wrappedCurrentFile = pCurrentFile;
    int indexOfLastDir = -1;
    for(int index = wrappedCurrentFile.length() - 1; index > -1 && indexOfLastDir == -1; --index)
    {
        if(wrappedCurrentFile[index] == '/' || wrappedCurrentFile[index] == '\'')
        {
            indexOfLastDir = index;
        }
    }
    if(indexOfLastDir != -1)
    {
        return wrappedCurrentFile.substr(0, indexOfLastDir) + fileName; 
    }
    return "";
}

}
}

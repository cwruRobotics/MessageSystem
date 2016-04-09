// SYSTEM INCLUDES
#include <atomic>
#include <mutex>

// C++ PROJECT INCLUDES
#include "Utilities/PythonBridge.hpp"
#include "Utilities/OSUtils.hpp"

namespace Embedded
{
namespace Bridges
{

    std::mutex          pythonLock;
    std::atomic<bool>   pythonStarted(false);

    std::string PathJoin(std::vector<std::string>& pathsToAdd)
    {
        std::string joinedPaths;
        int sizeToReserve = 0;
        for(std::string& val : pathsToAdd)
        {
            sizeToReserve += val.size();
        }
        joinedPaths.reserve(sizeToReserve + pathsToAdd.size());
        joinedPaths.append(Utilities::OS::GetPathDelimiter());
        for(int i = 0; i < pathsToAdd.size(); ++i)
        {
            joinedPaths.append(pathsToAdd[i]);
            if(i < pathsToAdd.size() - 1)
            {
                joinedPaths.append(Utilities::OS::GetPathDelimiter());
            }
        }
        return joinedPaths;
    }

    bool StartPython(std::vector<std::string> pathsToAdd)
    {
        std::string joinedPaths = PathJoin(pathsToAdd);
        std::lock_guard<std::mutex> lock(pythonLock);
        if(!pythonStarted)
        {
            Py_Initialize();
            PySys_SetPath((char*)(Py_GetPath() +
                                  joinedPaths).c_str());
            pythonStarted = true;
        }
        return pythonStarted;
    }

    bool StopPython()
    {
        std::lock_guard<std::mutex> lock(pythonLock);
        if(pythonStarted)
        {
            Py_Finalize();
        }
        return pythonStarted;
    }

} // end of namespace Bridges
} // end of namespace Embedded

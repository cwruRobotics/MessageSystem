// DO NOT TOUCH THIS LINE. PYTHON DEFINES SOME SYSTEM SPECIFIC VARIABLES ON UNIX.
// THAT GCC GENERATES WARNINGS FOR (THEY HAVE NO EFFECT ON PERFORMANCE), WHICH
// WILL FAIL THE BUILD. BY PUTTING THIS INCLUDE BEFORE SYSTEM INCLUDES WE CAN
// PREVENT THE WARNINGS.
#include "Utilities/PythonBridge.hpp"

// SYSTEM INCLUDES
#include <atomic>
#include <iostream>
#include <mutex>

// C++ PROJECT INCLUDES
#include "Utilities/OSUtils.hpp"

namespace Embedded
{
namespace Bridges
{

    std::mutex          pythonLock;
    PyThreadState*      mainState = nullptr;
    std::atomic<bool>   pythonStarted(false);
    std::atomic<bool>   pythonUsed(false);

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
            PyEval_InitThreads();
            PySys_SetPath((char*)(Py_GetPath() +
                                  joinedPaths).c_str());
            pythonStarted = true;
            mainState = PyEval_SaveThread();
        }
        return pythonStarted;
    }

    bool StopPython()
    {
        std::lock_guard<std::mutex> lock(pythonLock);
        if(pythonStarted)
        {
            std::cout << "Restoring Python Thread" << std::endl;
            PyEval_RestoreThread(mainState);

            std::cout << "Shutting down Python" << std::endl;
            Py_Finalize();
            mainState = nullptr;
        }
        return pythonStarted;
    }

    PyThreadState* GetGlobalPythonState()
    {
        return mainState;
    }

    PyObject* CallPythonFunctionWithArgs(PyObject* pModule, PyObject* pArgs, const char* pFuncName)
    {
        pythonUsed = true;
        PyObject* pFunc = nullptr, * pValue = nullptr;
        pFunc = PyObject_GetAttrString(pModule, (char*)pFuncName);
        Py_INCREF(pFunc);

        if(pFunc && PyCallable_Check(pFunc))
        {
            pValue = PyObject_CallObject(pFunc, pArgs);
            if(PyErr_Occurred())
            {
                PyErr_Print();
            }
        }
        else
        {
            if(PyErr_Occurred())
            {
                PyErr_Print();
            }
            else
            {
                // log error here.
            }
        }
        Py_XDECREF(pFunc);
        //Py_XDECREF(pArgs);
        return pValue;
    }

    PyObject* RunPythonFile(std::string fileName, PyObject* pArgs)
    {
        pythonUsed = true;
        PyObject* pName, *pModule, *pValue = nullptr;
        pName = PyString_FromString((char*)fileName.c_str());
        pModule = PyImport_Import(pName);

        if(pModule)
        {
            pValue = CallPythonFunctionWithArgs(pModule, pArgs, "main");
            if(!pValue && PyErr_Occurred())
            {
                PyErr_Print();
            }
            Py_DECREF(pModule);
        }
        else
        {
            if(PyErr_Occurred())
            {
                PyErr_Print();
            }
        }
        Py_XDECREF(pArgs);
        return pValue;
    }

} // end of namespace Bridges
} // end of namespace Embedded

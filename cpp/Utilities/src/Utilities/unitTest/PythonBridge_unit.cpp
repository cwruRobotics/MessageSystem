// DO NOT TOUCH THIS LINE. PYTHON DEFINES SOME SYSTEM SPECIFIC VARIABLES ON UNIX.
// THAT GCC GENERATES WARNINGS FOR (THEY HAVE NO EFFECT ON PERFORMANCE), WHICH
// WILL FAIL THE BUILD. BY PUTTING THIS INCLUDE BEFORE SYSTEM INCLUDES WE CAN
// PREVENT THE WARNINGS.
#include "Utilities/PythonInclude.hpp"

// SYSTEM INCLUDES
#include <iostream>
#include <thread>
#include <vector>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"

#include "Utilities/PythonBridge.hpp"
#include "Utilities/OSUtils.hpp"

namespace OSUtils = Utilities::OS;

namespace Embedded
{
namespace Bridges
{
namespace Tests
{

    TEST_CASE("Testing StartPython()", "[PythonBridge_unit]")
    {
        std::vector<std::string> paths;
        paths.push_back(OSUtils::GetCurrentDirectory(__FILE__));
        REQUIRE( StartPython(paths) );
        //PyRun_SimpleString("import sys; print(sys.path)");
    }

    PyObject* CallPythonFunctionWithArgs(PyObject* pModule, PyObject* pArgs, std::string func)
    {
        PyObject* pFunc, *pValue = nullptr, *pDict;
        pFunc = PyObject_GetAttrString(pModule, (char*)func.c_str());
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
            std::cout << "Cannot find function VoidFunctionNoArgs" << std::endl;
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
        return pValue;
    }

    /*
    TEST_CASE("Running a file", "[PythonBridge_unit]")
    {
        PyEval_RestoreThread(GetGlobalPythonState());

        PyObject* pName, *pModule, *pDict, *pFunc, *pValue;

        pName = PyString_FromString("Plugin");
        pModule = PyImport_Import(pName);

        if (pModule != nullptr)
        {
            SECTION("Calling VoidFunctionNoArgs")
            {
                pValue = CallPythonFunctionWithArgs(pModule, nullptr, "VoidFunctionNoArgs");
                Py_DECREF(pValue);
            }
            SECTION("Calling VoidFunctionOneArg")
            {
                PyObject* pArgs = PyTuple_New(1);
                PyTuple_SetItem(pArgs, 0, PyInt_FromLong(10));
                pValue = CallPythonFunctionWithArgs(pModule, pArgs, "VoidFunctionOneArg");
                Py_DECREF(pArgs);
                Py_DECREF(pValue);
            }
            SECTION("Calling VoidFunctionTwoArgs")
            {
                PyObject* pArgs = PyTuple_New(2);
                PyTuple_SetItem(pArgs, 0, PyInt_FromLong(10));
                PyTuple_SetItem(pArgs, 1, PyInt_FromLong(15));
                pValue = CallPythonFunctionWithArgs(pModule, pArgs, "VoidFunctionTwoArgs");
                Py_DECREF(pArgs);
                Py_DECREF(pValue);
            }
            SECTION("Calling ReturnFunctionNoArgs")
            {
                pValue = CallPythonFunctionWithArgs(pModule, nullptr, "ReturnFunctionNoArgs");
                REQUIRE( PyInt_AsLong(pValue) == 50 );
                Py_DECREF(pValue);
            }
            SECTION("Calling ReturnFunctionOneArg")
            {
                PyObject* pArgs = PyTuple_New(1);
                PyTuple_SetItem(pArgs, 0, PyInt_FromLong(10));
                pValue = CallPythonFunctionWithArgs(pModule, pArgs, "ReturnFunctionOneArg");
                Py_DECREF(pArgs);
                REQUIRE( PyInt_AsLong(pValue) == 15 );
                Py_DECREF(pValue);
            }
            SECTION("Calling ReturnFunctionTwoArgs")
            {
                PyObject* pArgs = PyTuple_New(2);
                PyTuple_SetItem(pArgs, 0, PyInt_FromLong(10));
                PyTuple_SetItem(pArgs, 1, PyInt_FromLong(15));
                pValue = CallPythonFunctionWithArgs(pModule, pArgs, "ReturnFunctionTwoArgs");
                Py_DECREF(pArgs);
                REQUIRE( PyInt_AsLong(pValue) == 25 );
                Py_DECREF(pValue);
            }
            SECTION("Calling Python main")
            {
                pValue = CallPythonFunctionWithArgs(pModule, nullptr, "main");
                REQUIRE( PyInt_AsLong(pValue) == 5 );
                Py_DECREF(pValue);
            }
        }
        else
        {
            PyErr_Print();
            std::cout << "Failed to load Plugin.py" << std::endl;
        }
    }

    TEST_CASE("Testing RunPythonFile", "[PythonBridge_unit]")
    {
        PyObject* pValue = RunPythonFile("Plugin", nullptr);
        if(pValue)
        {
            REQUIRE( PyInt_AsLong(pValue) == 5 );
        }
        else
        {
            REQUIRE( false );
        }
        Py_XDECREF(pValue);
        PyEval_SaveThread();
    }

    */
    TEST_CASE("Testing StopPython()", "[PythonBridge_unit]")
    {
        REQUIRE( StopPython() );
    }

} // end of namespace Embedded
} // end of namespace Bridges
} // end of namespace Tests

#include "Robos/PyNodeBase.hpp"

// SYSTEM INCLUDES
#include <iostream>

// C++ PROJECT INCLUDES

namespace Robos
{

    PyNodeBase::PyNodeBase(std::string name, std::string toRunOn,
                           std::string pythonFileName,
                           std::vector<std::string> subscriptions) :
        NodeBase(name, toRunOn, subscriptions), _pythonFileName(pythonFileName), _moduleLoaded(false), _pModuleName(nullptr),
        _pModule(nullptr)
    {
    }

    PyNodeBase::~PyNodeBase()
    {
    }

    MessageBasePtr PyNodeBase::MainCallbackImpl(const MessageBasePtr pMessage)
    {
        PyGILState_STATE state;
        state = PyGILState_Ensure();

        if(!_moduleLoaded)
        {
            try
            {
                _moduleLoaded = true;
                this->_pModuleName = PyString_FromString((char*)this->_pythonFileName.c_str());
                this->_pModule = PyImport_Import(this->_pModuleName);
            }
            catch(...)
            {
                std::cout << "Error loading module" << std::endl;
                PyGILState_Release(state);
                return nullptr;
            }
        }
        

        try
        {
            PyObject* pArgs = this->PackArgs(pMessage);

            PyObject* pReturnValues = Embedded::Bridges::CallPythonFunctionWithArgs(this->_pModule, pArgs, "main");
            MessageBasePtr pNextMessage = this->UnpackPythonObject( pReturnValues);

            Py_XDECREF(pArgs);

            Py_XDECREF(pReturnValues);

            PyGILState_Release(state);
            return pNextMessage;
        }
        catch(std::exception e)
        {
            std::cout << "Error caught: " << e.what() << std::endl;
            PyGILState_Release(state);
        }
        catch(...)
        {
            std::cout << "Error caught....releasing GIL" << std::endl;
            PyGILState_Release(state);
        }
        return nullptr;
    }

}

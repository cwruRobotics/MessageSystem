// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/PyNodeBase.hpp"

namespace Robos
{

    PyNodeBase::PyNodeBase(std::string name, std::string toRunOn,
                           std::string pythonFileName,
                           std::vector<std::string> subscriptions) :
        NodeBase(name, toRunOn, subscriptions), _pythonFileName(pythonFileName)
    {
    }

    PyNodeBase::~PyNodeBase()
    {
    }

    MessageBasePtr PyNodeBase::MainCallbackImpl(const MessageBasePtr pMessage)
    {
        PyGILState_STATE globalState;
        globalState = PyGILState_Ensure();

        PyObject* pArgs = this->PackArgs(pMessage);
        PyObject* pReturnValues = Embedded::Bridges::RunPythonFile(this->_pythonFileName,
                                                                   pArgs);
        MessageBasePtr pNextMessage = this->UnpackPythonObject(pReturnValues);
        Py_XDECREF(pArgs);
        Py_XDECREF(pReturnValues);

        PyGILState_Release(globalState);
        return pNextMessage;
    }

}

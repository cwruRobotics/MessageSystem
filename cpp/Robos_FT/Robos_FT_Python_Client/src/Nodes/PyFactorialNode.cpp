// SYSTEM INCLUDES
//#include <stdexcept>
#include <iostream>

// C++ PROJECT INCLUDES
#include "Nodes/PyFactorialNode.hpp"
#include "Messages/FactorialMessage.hpp"
#include "Messages/SensorMessage.hpp"

namespace Robos_FT_Cpp_Client
{

    PyFactorialNode::PyFactorialNode() : Robos::PyNodeBase("FactorialNode", "FactorialScheduler",
        "Factorial", std::vector<std::string>{"SensorMessage"}),
        _factorialArg(0)
    {
    }

    PyFactorialNode::~PyFactorialNode()
    {
    }

    Robos::MessageBasePtr PyFactorialNode::UnpackPythonObject(PyObject* pObject)
    {
        int result = PyInt_AsLong(pObject);
        FactorialMessagePtr pMessage = std::make_shared<FactorialMessage>(this->_factorialArg, result);
        return pMessage;
    }

    PyObject* PyFactorialNode::PackArgs(const Robos::MessageBasePtr pMessage)
    {
        if(pMessage->topic == "SensorMessage")
        {
            auto pCasted = std::dynamic_pointer_cast<SensorMessage>(pMessage);
            this->_factorialArg = pCasted->numToFactorial;
            PyObject* pArgs = PyTuple_New(1);
            PyTuple_SetItem(pArgs, 0, PyInt_FromLong(pCasted->numToFactorial));
            return pArgs;
        }
        throw std::logic_error("Unknown Message topic " + pMessage->topic);
    }

} // end of namespace Robos_FT_Cpp_Client
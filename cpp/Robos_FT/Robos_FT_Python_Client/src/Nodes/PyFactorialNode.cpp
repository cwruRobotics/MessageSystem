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
        // we expect Python's result to be an int.
        int result = PyInt_AsLong(pObject);

        // package up everything into a FactorialMessage
        FactorialMessagePtr pMessage = std::make_shared<FactorialMessage>(this->_factorialArg, result);
        return pMessage;
    }

    PyObject* PyFactorialNode::PackArgs(const Robos::MessageBasePtr pMessage)
    {
        if(pMessage->topic == "SensorMessage")
        {
            // cast Message to appropriate type
            auto pCasted = std::dynamic_pointer_cast<SensorMessage>(pMessage);

            // save factorial arg for later (will stuff into FactorialMessage later)
            this->_factorialArg = pCasted->numToFactorial;

            // create a new Python tuple
            PyObject* pArgs = PyTuple_New(1);

            // fill the tuple with the value we want to give python.
            PyTuple_SetItem(pArgs, 0, PyInt_FromLong(pCasted->numToFactorial));
            return pArgs;
        }
        throw std::logic_error("Unknown Message topic " + pMessage->topic);
    }

} // end of namespace Robos_FT_Cpp_Client
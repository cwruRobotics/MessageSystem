#pragma once
#ifndef ROBOS_PYNODE_HPP
#define ROBOS_PYNODE_HPP

// SYSTEM INCLUDES
#include <Utilities/PythonBridge.hpp>

// C++ PROJECT INCLUDES
#include "Robos/LibraryExport.hpp"
#include "Robos/NodeBase.hpp"

/**
 * @namespace Robos The Robos namespace.
 */
namespace Robos
{

    /**
     * Base class for client code that is written in Python.
     * Given that clients have followed the instructions in the /manual/ directory to set up
     * the Python interpreter (default Python installations are not enough for embedding!)
     * then this class can be derived to write custom Python interfacing calls including
     * data type conversions between C++ and Python.
     */
    class ROBOS_API PyNodeBase : public NodeBase
    {
    private:

        const std::string               _pythonFileName;
        bool                            _moduleLoaded;
        PyObject*                       _pModuleName;
        PyObject*                       _pModule;

        /**
         * THIS METHOD MUST BE OVERRIDEN BY CLIENTS.
         * Method to turn a pointer to a Python Object (PyObject*) into a MessageBase.
         * This method is called after the service call into Python has been made and
         * is used to unpack the results from Python into a MessageBase.
         * @param[in]   pObject         The Python object that needs to be unpacked.
         * @param[out]  MessageBasePtr  The message that will be published to Robos.
         */
        virtual MessageBasePtr UnpackPythonObject(PyObject* pObject) = 0;

        /**
         * THIS METHOD MUST BE OVERRIDEN BY CLIENTS.
         * Method to turn a message into a pointer to a Python Object. This method is called
         * to convert an incoming message into data types that Python can use. The result
         * of this method are given to Python to execute client Python code.
         */
        virtual PyObject* PackArgs(const MessageBasePtr pMessage) = 0;

        /**
         * Client code customization method. This method cannot be overriden (marked "final").
         * This method will call PackArgs(), then will execute the pythonFileName supplied
         * by the constructor (calling a function named "main") with the result of PackArgs(),
         * and will call UnpackPythonObject() with the result of the python call. The result
         * of UnpackPythonObject() will be a MessageBasePtr and will publish that message.
         *
         * Here is a summary of what happens in this method:
         *
         * MainCallbackImpl(const MessageBasePtr pMessage):
         *      publish UnpackPythonObject(python call pythonFileName.main(PackArgs(pMessage)))
         */
        virtual MessageBasePtr MainCallbackImpl(const MessageBasePtr pMessage) override final;

    public:

        /**
         * A constructor.
         * @param[in]   name            The name of this instance. This should be unique in that
         *                              there cannot be a Node with the same name that also
         *                              shares a subscription.
         * @param[in]   toRunOn         The name of the Scheduler that should manage this instance.
         * @param[in]   pythonFileName  The name of the python file that contains the clients
         *                              "main" function.
         * @param[in]   subscriptions   A list of message topics that this instance subscribes to.
         */
        PyNodeBase(std::string name, std::string toRunOn,
                   std::string pythonFileName,
                   std::vector<std::string> subscriptions);

        /**
         * A destructor. Virtual to allow inheritance.
         */
        virtual ~PyNodeBase();

    };

} // end of namespace Robos

#endif // end of ROBOS_PYNODE_HPP

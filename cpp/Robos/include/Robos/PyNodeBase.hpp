#pragma once
#ifndef ROBOS_PYNODE_HPP
#define ROBOS_PYNODE_HPP

// SYSTEM INCLUDES
#include <Utilities/PythonBridge.hpp>

// C++ PROJECT INCLUDES
#include "Robos/LibraryExport.hpp"
#include "Robos/NodeBase.hpp"

namespace Robos
{

    class ROBOS_API PyNodeBase : public NodeBase
    {
    private:

        const std::string               _pythonFileName;
        bool                            _moduleLoaded;
        PyObject*                       _pModuleName;
        PyObject*                       _pModule;

        virtual MessageBasePtr UnpackPythonObject(PyObject* pObject) = 0;

        virtual PyObject* PackArgs(const MessageBasePtr pMessage) = 0;

        virtual MessageBasePtr MainCallbackImpl(const MessageBasePtr pMessage) override;

    public:

        PyNodeBase(std::string name, std::string toRunOn,
                   std::string pythonFileName,
                   std::vector<std::string> subscriptions);

        virtual ~PyNodeBase();

    };

} // end of namespace Robos

#endif // end of ROBOS_PYNODE_HPP

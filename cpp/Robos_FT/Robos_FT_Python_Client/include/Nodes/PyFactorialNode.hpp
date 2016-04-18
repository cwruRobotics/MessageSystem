#pragma once
#ifndef ROBOS_FT_CPP_CLIENT_PYFACTORIALNODE_HPP
#define ROBOS_FT_CPP_CLIENT_PYFACTORIALNODE_HPP

// SYSTEM INCLUDES
#include <Robos/PyNodeBase.hpp>
#include <Robos/MessageBase.hpp>

// C++ PROJECT INCLUDES


namespace Robos_FT_Cpp_Client
{

    class PyFactorialNode : public Robos::PyNodeBase
    {
    private:
        int _factorialArg;

        virtual Robos::MessageBasePtr UnpackPythonObject(PyObject* pObject) override;

        virtual PyObject* PackArgs(const Robos::MessageBasePtr pMessage) override;

    public:

        PyFactorialNode();

        ~PyFactorialNode();

    };

    using PyFactorialNodePtr = std::shared_ptr<PyFactorialNode>;

} // end of namespace Robos_FT_Cpp_Client

#endif // end of ROBOS_FT_CPP_CLIENT_PYFACTORIALNODE_HPP

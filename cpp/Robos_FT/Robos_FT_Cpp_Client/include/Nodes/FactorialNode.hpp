#pragma once
#ifndef ROBOS_FT_CPP_CLIENT_FACTORIALNODE_HPP
#define ROBOS_FT_CPP_CLIENT_FACTORIALNODE_HPP

// SYSTEM INCLUDES
#include <Robos/NodeBase.hpp>
#include <Robos/MessageBase.hpp>

// C++ PROJECT INCLUDES


namespace Robos_FT_Cpp_Client
{

    class FactorialNode : public Robos::NodeBase
    {
    private:

        int ComputeFactorial(int n);

        virtual Robos::MessageBasePtr MainCallbackImpl(const Robos::MessageBasePtr pMessage) override;

    public:

        FactorialNode();

        ~FactorialNode();

    };

    using FactorialNodePtr = std::shared_ptr<FactorialNode>;

} // end of namespace Robos_FT_Cpp_Client

#endif // end of ROBOS_FT_CPP_CLIENT_FACTORIALNODE_HPP

#pragma once
#ifndef ROBOS_FT_CPP_CLIENT_FACTORIALMESSAGE_HPP
#define ROBOS_FT_CPP_CLIENT_FACTORIALMESSAGE_HPP

// SYSTEM INCLUDES
#include <Robos/MessageBase.hpp>

// C++ PROJECT INCLUDES


namespace Robos_FT_Cpp_Client
{

    struct FactorialMessage : public Robos::MessageBase
    {
        int factorialResult;

        int factorialArg;

        FactorialMessage(int arg, int result);

        ~FactorialMessage();
    };

    using FactorialMessagePtr = std::shared_ptr<FactorialMessage>;

} // end of namespace Robos_FT_Cpp_Client

#endif // end of ROBOS_FT_CPP_CLIENT_FACTORIALMESSAGE_HPP

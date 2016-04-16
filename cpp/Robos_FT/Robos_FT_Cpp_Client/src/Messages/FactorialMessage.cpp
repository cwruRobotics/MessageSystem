// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Messages/FactorialMessage.hpp"

namespace Robos_FT_Cpp_Client
{

    FactorialMessage::FactorialMessage(int arg, int result) :
        Robos::MessageBase("FactorialMessage"), factorialArg(arg), factorialResult(result)
    {
    }

    FactorialMessage::~FactorialMessage()
    {
    }

} // end of namespace Robos_FT_Cpp_Client

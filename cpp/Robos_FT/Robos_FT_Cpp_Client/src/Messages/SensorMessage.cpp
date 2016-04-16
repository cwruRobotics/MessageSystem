// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Messages/SensorMessage.hpp"


namespace Robos_FT_Cpp_Client
{

    SensorMessage::SensorMessage(int n) : Robos::MessageBase("SensorMessage"), numToFactorial(n)
    {
    }

    SensorMessage::~SensorMessage()
    {
    }

} // end of namespace Robos_FT_Cpp_Client

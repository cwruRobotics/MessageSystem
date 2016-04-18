#pragma once
#ifndef ROBOS_FT_CPP_CLIENT_SENSORMESSAGE_HPP
#define ROBOS_FT_CPP_CLIENT_SENSORMESSAGE_HPP

// SYSTEM INCLUDES
#include <Robos/MessageBase.hpp>

// C++ PROJECT INCLUDES


namespace Robos_FT_Cpp_Client
{

    struct SensorMessage : public Robos::MessageBase
    {
        int numToFactorial;

        SensorMessage(int n);

        ~SensorMessage();

    };

    using SensorMessagePtr = std::shared_ptr<SensorMessage>;

} // end of namespace Robos_FT_Cpp_Client

#endif // end of ROBOS_FT_CPP_CLIENT_SENSORMESSAGE_HPP

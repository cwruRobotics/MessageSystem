#pragma once
#ifndef ROBOS_FT_CPP_CLIENT_SENSORNODE_HPP
#define ROBOS_FT_CPP_CLIENT_SENSORNODE_HPP

// SYSTEM INCLUDES
#include <Robos/InitNodeBase.hpp>


// C++ PROJECT INCLUDES


namespace Robos_FT_Cpp_Client
{

    class SensorNode : public Robos::InitNodeBase
    {
    private:

        virtual void MainCallbackImpl() override;

    public:

        SensorNode();

        ~SensorNode();

    };

    using SensorNodePtr = std::shared_ptr<SensorNode>;

} // end of namespace Robos_FT_Cpp_Client

#endif // end of ROBOS_FT_CPP_CLIENT_SENSORNODE_HPP

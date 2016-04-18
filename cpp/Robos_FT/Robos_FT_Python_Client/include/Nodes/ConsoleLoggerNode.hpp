#pragma once
#ifndef ROBOS_FT_CPP_CLIENT_CONSOLELOGGERNODE_HPP
#define ROBOS_FT_CPP_CLIENT_CONSOLELOGGERNODE_HPP

// SYSTEM INCLUDES
#include <Robos/NodeBase.hpp>

// C++ PROJECT INCLUDES


namespace Robos_FT_Cpp_Client
{

    class ConsoleLoggerNode : public Robos::NodeBase
    {
    private:

        virtual Robos::MessageBasePtr MainCallbackImpl(const Robos::MessageBasePtr pMessage) override;

    public:

        ConsoleLoggerNode();

        ~ConsoleLoggerNode();

    };

    using ConsoleLoggerNodePtr = std::shared_ptr<ConsoleLoggerNode>;

} // end of namespace Robos_FT_Cpp_Client

#endif // end of ROBOS_FT_CPP_CLIENT_CONSOLELOGGERNODE_HPP

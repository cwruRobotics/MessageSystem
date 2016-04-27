#pragma once
#ifndef ROBOS_TESTS_SHUTDOWNTESTNODE_HPP
#define ROBOS_TESTS_SHUTDOWNTESTNODE_HPP


// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/InitNodeBase.hpp"

namespace Robos
{
namespace Tests
{

    class ShutdownTestNode : public InitNodeBase
    {
    private:

        void MainCallbackImpl() override;

    public:

        ShutdownTestNode();

        ~ShutdownTestNode();

    };

    using ShutdownTestNodePtr = std::shared_ptr<ShutdownTestNode>;

} // end of namespace Tests
} // end of namespace Robos

#endif // end of ROBOS_TESTS_SHUTDOWNTESTNODE_HPP

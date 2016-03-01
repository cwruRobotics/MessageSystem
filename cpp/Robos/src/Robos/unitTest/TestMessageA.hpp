#ifndef ROBOS_TESTS_TESTMESSAGEA_HPP
#define ROBOS_TESTS_TESTMESSAGEA_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/MessageBase.hpp"

namespace Robos
{
namespace Tests
{

    struct TestMessageA : public MessageBase
    {
        int data;

        TestMessageA();

        ~TestMessageA();

    };

    using TestMessageAPtr = std::shared_ptr<TestMessageA>;

} // end of namespace Tests
} // end of namespace Robos

#endif // end of ROBOS_TESTS_TESTMESSAGEA_HPP

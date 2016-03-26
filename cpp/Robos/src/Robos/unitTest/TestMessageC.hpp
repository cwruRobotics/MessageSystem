#ifndef ROBOS_TESTS_TESTMESSAGEC_HPP
#define ROBOS_TESTS_TESTMESSAGEC_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/MessageBase.hpp"

namespace Robos
{
namespace Tests
{

    struct TestMessageC : public MessageBase
    {
        int data;

        TestMessageC();

        ~TestMessageC();

    };

    using TestMessageCPtr = std::shared_ptr<TestMessageC>;

} // end of namespace Tests
} // end of namespace Robos

#endif // end of ROBOS_TESTS_TESTMESSAGEC_HPP

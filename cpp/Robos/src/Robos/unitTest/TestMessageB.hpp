#ifndef ROBOS_TESTS_TESTMESSAGEB_HPP
#define ROBOS_TESTS_TESTMESSAGEB_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/MessageBase.hpp"

namespace Robos
{
namespace Tests
{

    struct TestMessageB : public MessageBase
    {
        int data;

        TestMessageB();

        ~TestMessageB();

    };

    using TestMessageBPtr = std::shared_ptr<TestMessageB>;

} // end of namespace Tests
} // end of namespace Robos

#endif // end of ROBOS_TESTS_TESTMESSAGEB_HPP

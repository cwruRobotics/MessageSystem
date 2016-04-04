#ifndef ROBOS_TESTS_TESTCOUNTMESSAGE_HPP
#define ROBOS_TESTS_TESTCOUNTMESSAGE_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/MessageBase.hpp"

namespace Robos
{
namespace Tests
{

    struct TestCountMessage : public MessageBase
    {
        int* count;

        TestCountMessage();

        ~TestCountMessage();

    };

    using TestCountMessagePtr = std::shared_ptr<TestCountMessage>;

} // end of namespace Tests
} // end of namespace Robos

#endif // end of ROBOS_TESTS_TESTCOUNTMESSAGE_HPP

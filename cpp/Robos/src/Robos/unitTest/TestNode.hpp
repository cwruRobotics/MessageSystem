#ifndef ROBOS_TESTS_TESTNODE_HPP
#define ROBOS_TESTS_TESTNODE_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/NodeBase.hpp"
#include "Robos/unitTest/TestMessageA.hpp"
#include "Robos/unitTest/TestMessageB.hpp"

namespace Robos
{
namespace Tests
{

    class TestNode : public NodeBase
    {
    private:

        virtual MessageBasePtr MainCallbackImpl(const MessageBasePtr& pMessage) override;

        MessageBasePtr TestNodeCallback(const TestMessageAPtr& pMessage);

    public:

        TestNode();

        ~TestNode();
    };

    using TestNodePtr = std::shared_ptr<TestNode>;

} // end of namespace Tests
} // end of namespace Robos


#endif // end of ROBOS_TESTS_TESTNODE_HPP

#ifndef ROBOS_TESTS_TESTNODENAMETEMPLATE_HPP
#define ROBOS_TESTS_TESTNODENAMETEMPLATE_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/NodeBase.hpp"
#include "Robos/unitTest/TestMessageA.hpp"
#include "Robos/unitTest/TestMessageB.hpp"

namespace Robos
{
namespace Tests
{

    class TestNodeNameTemplate : public NodeBase
    {
    private:

        virtual MessageBasePtr MainCallbackImpl(const MessageBasePtr pMessage) override;

        MessageBasePtr TestNodeCallback(const TestMessageAPtr pMessage);

    public:

        TestNodeNameTemplate(const std::string name);

        ~TestNodeNameTemplate();
    };

    using TestNodeNameTemplatePtr = std::shared_ptr<TestNodeNameTemplate>;

} // end of namespace Tests
} // end of namespace Robos


#endif // end of ROBOS_TESTS_TESTNODENAMETEMPLATE_HPP

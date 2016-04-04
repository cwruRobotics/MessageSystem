#ifndef ROBOS_TESTS_TESTNODESUBSCRIBERTEMPLATE_HPP
#define ROBOS_TESTS_TESTNODESUBSCRIBERTEMPLATE_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/NodeBase.hpp"
#include "Robos/unitTest/TestMessageA.hpp"
#include "Robos/unitTest/TestMessageB.hpp"
#include "Robos/unitTest/TestMessageC.hpp"

namespace Robos
{
namespace Tests
{

    class TestNodeSubscriberTemplate : public NodeBase
    {
    private:

        virtual MessageBasePtr MainCallbackImpl(const MessageBasePtr pMessage) override;

        MessageBasePtr TestNodeCallback(const TestMessageAPtr pMessage);

        MessageBasePtr TestNodeCallbackB(const TestMessageBPtr pMessage);

    public:

        TestNodeSubscriberTemplate(const std::vector<std::string> subscriptions);

        ~TestNodeSubscriberTemplate();
    };

    using TestNodeSubscriberTemplatePtr = std::shared_ptr<TestNodeSubscriberTemplate>;

} // end of namespace Tests
} // end of namespace Robos


#endif // end of ROBOS_TESTS_TESTNODESUBSCRIBERTEMPLATE_HPP

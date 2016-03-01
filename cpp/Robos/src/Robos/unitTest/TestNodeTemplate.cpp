// SYSTEM INCLUDES
// #include <Utilities/VectorUtils.hpp>

// C++ PROJECT INCLUDES
#include "Robos/unitTest/TestNodeTemplate.hpp"
#include "catch/catch.hpp"

namespace Robos
{
namespace Tests
{

    TestNodeTemplate::TestNodeTemplate(const std::string name) : NodeBase(name, "testScheduler",
        std::vector<std::string>{"testTopicA"})
    {
    }

    TestNodeTemplate::~TestNodeTemplate()
    {
    }

    MessageBasePtr TestNodeTemplate::TestNodeCallback(const TestMessageAPtr& pMessage)
    {
        TestMessageBPtr producedMessage = std::make_shared<TestMessageB>();
        REQUIRE( pMessage->data == 10 );
        producedMessage->data = pMessage->data + 5;
        return producedMessage;
    }

    MessageBasePtr TestNodeTemplate::MainCallbackImpl(const MessageBasePtr& pMessage)
    {
        if(pMessage->topic == "testTopicA")
        {
            return this->TestNodeCallback(std::dynamic_pointer_cast<TestMessageA>(pMessage));
        }
        else
        {
            throw std::logic_error("Unknown message topic: [" + pMessage->topic + "]");
        }
    }

} // end of namespace Tests
} // end of namespace Robos

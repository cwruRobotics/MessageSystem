// SYSTEM INCLUDES
// #include <Utilities/VectorUtils.hpp>
#include <iostream>
#include <cassert>

// C++ PROJECT INCLUDES
#include "Robos/unitTest/TestNodeSubscriberTemplate.hpp"
#include "catch/catch.hpp"

namespace Robos
{
namespace Tests
{

    TestNodeSubscriberTemplate::TestNodeSubscriberTemplate(const std::vector<std::string> subscriptions) :
        NodeBase("testNode", "TestScheduler", subscriptions)
    {
    }

    TestNodeSubscriberTemplate::~TestNodeSubscriberTemplate()
    {
    }

    MessageBasePtr TestNodeSubscriberTemplate::TestNodeCallback(const TestMessageAPtr pMessage)
    {
        TestMessageBPtr producedMessage = std::make_shared<TestMessageB>();
        REQUIRE( pMessage->data == 10 );
        producedMessage->data = pMessage->data + 5;
        return producedMessage;
    }

    MessageBasePtr TestNodeSubscriberTemplate::TestNodeCallbackB(const TestMessageBPtr pMessage)
    {
        TestMessageAPtr producedMessage = std::make_shared<TestMessageA>();
        REQUIRE( pMessage->data == 15 );
        producedMessage->data = pMessage->data - 5;
        return producedMessage;
    }

    MessageBasePtr TestNodeSubscriberTemplate::MainCallbackImpl(const MessageBasePtr pMessage)
    {
        assert(pMessage);
        // std::cout << "Executing TestNodeSubscriberTemplate with topic [" << pMessage->topic << "]" << std::endl;
        if(pMessage->topic == "testTopicA")
        {
            return this->TestNodeCallback(std::dynamic_pointer_cast<TestMessageA>(pMessage));
        }
        else if(pMessage->topic == "testTopicB")
        {
            return this->TestNodeCallbackB(std::dynamic_pointer_cast<TestMessageB>(pMessage));
        }
        else
        {
            throw std::logic_error("Unknown message topic: [" + pMessage->topic + "]");
        }
    }

} // end of namespace Tests
} // end of namespace Robos

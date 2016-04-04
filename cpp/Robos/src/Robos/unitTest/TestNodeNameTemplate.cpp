// SYSTEM INCLUDES
// #include <Utilities/VectorUtils.hpp>
#include <iostream>
#include <cassert>

// C++ PROJECT INCLUDES
#include "Robos/unitTest/TestNodeNameTemplate.hpp"
#include "catch/catch.hpp"

namespace Robos
{
namespace Tests
{

    TestNodeNameTemplate::TestNodeNameTemplate(const std::string name) : NodeBase(name, "TestScheduler",
        std::vector<std::string>{"testTopicA"})
    {
    }

    TestNodeNameTemplate::~TestNodeNameTemplate()
    {
    }

    MessageBasePtr TestNodeNameTemplate::TestNodeCallback(const TestMessageAPtr pMessage)
    {
        //std::cout << "TestNodeCallBack() pMessage use count: " << pMessage.use_count() << std::endl;
        TestMessageBPtr producedMessage = std::make_shared<TestMessageB>();
        REQUIRE( pMessage->data == 10 );
        producedMessage->data = pMessage->data + 5;
        return producedMessage;
    }

    MessageBasePtr TestNodeNameTemplate::MainCallbackImpl(const MessageBasePtr pMessage)
    {
        assert(pMessage);
        //std::cout << "Executing TestNodeNameTemplate" << this->GetName() << std::endl;
        // std::cout << "MainCallback() pMessage use count: " << pMessage.use_count() << std::endl;
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

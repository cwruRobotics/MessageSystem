// SYSTEM INCLUDES
// #include <Utilities/VectorUtils.hpp>
#include <iostream>

// C++ PROJECT INCLUDES
#include "Robos/unitTest/TestInitNode.hpp"
#include "catch/catch.hpp"

namespace Robos
{
namespace Tests
{

    TestInitNode::TestInitNode() : InitNodeBase("testNode", "TestScheduler")
    {
    }

    TestInitNode::~TestInitNode()
    {
    }

    MessageBasePtr TestInitNode::TestInitNodeCallback()
    {
        TestMessageBPtr producedMessage = std::make_shared<TestMessageB>();
        producedMessage->data = 15;
        return producedMessage;
    }

    void TestInitNode::MainCallbackImpl()
    {
        // std::cout << "Executing TestInitNodes" << std::endl;
        this->PublishMessage(this->TestInitNodeCallback());
    }

} // end of namespace Tests
} // end of namespace Robos

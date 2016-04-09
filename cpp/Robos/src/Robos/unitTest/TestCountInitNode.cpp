// SYSTEM INCLUDES
// #include <Utilities/VectorUtils.hpp>
#include <iostream>

// C++ PROJECT INCLUDES
#include "Robos/unitTest/TestCountInitNode.hpp"
#include "catch/catch.hpp"

namespace Robos
{
namespace Tests
{

    TestCountInitNode::TestCountInitNode(int* pCount) : InitNodeBase("TestCountInitNode", "TestScheduler2"), _pCount(pCount)
    {
    }

    TestCountInitNode::~TestCountInitNode()
    {
    }

    void TestCountInitNode::MainCallbackImpl()
    {
        auto producedMessage = std::make_shared<TestCountMessage>();
        producedMessage->count = _pCount;
        this->PublishMessage(producedMessage);
    }

} // end of namespace Tests
} // end of namespace Robos

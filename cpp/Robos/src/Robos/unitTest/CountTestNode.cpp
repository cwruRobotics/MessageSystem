// SYSTEM INCLUDES
// #include <Utilities/VectorUtils.hpp>
#include <iostream>
#include <cassert>

// C++ PROJECT INCLUDES
#include "Robos/unitTest/CountTestNode.hpp"
#include "catch/catch.hpp"

namespace Robos
{
namespace Tests
{

    CountTestNode::CountTestNode() : NodeBase("CountTestNode", "TestScheduler",
        std::vector<std::string>{"countTestTopic"})
    {
    }

    CountTestNode::~CountTestNode()
    {
    }

    MessageBasePtr CountTestNode::MainCallbackImpl(const MessageBasePtr pMessage)
    {
        if(pMessage->topic == "countTestTopic")
        {
            auto pCastedMessage = std::dynamic_pointer_cast<TestCountMessage>(pMessage);
            (*(pCastedMessage->count))++;
            return std::make_shared<TestMessageB>();
        }
        else
        {
            throw std::logic_error("Unknown message topic: [" + pMessage->topic + "]");
        }
    }

} // end of namespace Tests
} // end of namespace Robos

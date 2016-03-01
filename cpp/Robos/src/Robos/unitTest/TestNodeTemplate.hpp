#ifndef ROBOS_TESTS_TESTNODETEMPLATE_HPP
#define ROBOS_TESTS_TESTNODETEMPLATE_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/NodeBase.hpp"
#include "Robos/unitTest/TestMessageA.hpp"
#include "Robos/unitTest/TestMessageB.hpp"

namespace Robos
{
namespace Tests
{

    class TestNodeTemplate : public NodeBase
    {
    private:

        virtual MessageBasePtr MainCallbackImpl(const MessageBasePtr& pMessage) override;

        MessageBasePtr TestNodeCallback(const TestMessageAPtr& pMessage);

    public:

        TestNodeTemplate(const std::string name);

        ~TestNodeTemplate();
    };

    using TestNodeTemplatePtr = std::shared_ptr<TestNodeTemplate>;

} // end of namespace Tests
} // end of namespace Robos


#endif // end of ROBOS_TESTS_TESTNODETEMPLATE_HPP

#ifndef ROBOS_TESTS_COUNTTESTNODE_HPP
#define ROBOS_TESTS_COUNTTESTNODE_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/NodeBase.hpp"
#include "Robos/unitTest/TestCountMessage.hpp"
#include "Robos/unitTest/TestMessageB.hpp"

namespace Robos
{
namespace Tests
{

    class CountTestNode : public NodeBase
    {
    private:

        virtual MessageBasePtr MainCallbackImpl(const MessageBasePtr pMessage) override;

    public:

        CountTestNode();

        ~CountTestNode();
    };

    using CountTestNodePtr = std::shared_ptr<CountTestNode>;

} // end of namespace Tests
} // end of namespace Robos


#endif // end of ROBOS_TESTS_COUNTTESTNODE_HPP

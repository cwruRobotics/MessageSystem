#ifndef ROBOS_TESTS_TESTINITNODE_HPP
#define ROBOS_TESTS_TESTINITNODE_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/InitNodeBase.hpp"
#include "Robos/unitTest/TestMessageA.hpp"
#include "Robos/unitTest/TestMessageB.hpp"

namespace Robos
{
namespace Tests
{

    class TestInitNode : public InitNodeBase
    {
    private:

        virtual MessageBasePtr MainCallbackImpl() override;

        MessageBasePtr TestInitNodeCallback();

    public:

        TestInitNode();

        ~TestInitNode();
    };

    using TestInitNodePtr = std::shared_ptr<TestInitNode>;

} // end of namespace Tests
} // end of namespace Robos


#endif // end of ROBOS_TESTS_TESTINITNODE_HPP

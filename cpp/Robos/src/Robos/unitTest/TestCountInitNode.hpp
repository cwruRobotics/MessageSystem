#ifndef ROBOS_TESTS_TESTCOUNTINITNODE_HPP
#define ROBOS_TESTS_TESTCOUNTINITNODE_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/InitNodeBase.hpp"
#include "Robos/unitTest/TestCountMessage.hpp"

namespace Robos
{
namespace Tests
{

    class TestCountInitNode : public InitNodeBase
    {
    private:

        int* _pCount;

    private:

        virtual MessageBasePtr MainCallbackImpl() override;

    public:

        TestCountInitNode(int* pCount);

        ~TestCountInitNode();
    };

    using TestCountInitNodePtr = std::shared_ptr<TestCountInitNode>;

} // end of namespace Tests
} // end of namespace Robos


#endif // end of ROBOS_TESTS_TESTCOUNTINITNODE_HPP

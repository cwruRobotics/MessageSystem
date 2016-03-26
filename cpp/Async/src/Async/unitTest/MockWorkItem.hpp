#pragma once

#ifndef ASYNC_TESTS_MOCKWORKITEM_HPP
#define ASYNC_TESTS_MOCKWORKITEM_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/WorkItem.hpp"

namespace Async
{
namespace Tests
{

    class MockWorkItem : public WorkItem
    {
    public:

        MockWorkItem(uint64_t id=0, Types::JobPriority=Types::JobPriority::OTHER);

        ~MockWorkItem();

        void AttachMainFunction(FunctionPtr pFunc) override;

        void AttachPosteriorFunction(FunctionPtr pFunc) override;

    };

    using MockWorkItemPtr = std::shared_ptr<MockWorkItem>;

} // end of namespace Tests
} // end of namespace Async


#endif // end of ASYNC_TESTS_MOCKWORKITEM_HPP

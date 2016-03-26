// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/unitTest/MockWorkItem.hpp"

namespace Async
{
namespace Tests
{

    MockWorkItem::MockWorkItem(uint64_t id, Types::JobPriority priority) :  WorkItem(id, priority)
    {
    }

    MockWorkItem::~MockWorkItem()
    {
    }

    void MockWorkItem::AttachMainFunction(FunctionPtr pFunc)
    {
        this->WorkItem::AttachMainFunction(pFunc);
    }

    void MockWorkItem::AttachPosteriorFunction(FunctionPtr pFunc)
    {
        this->WorkItem::AttachPosteriorFunction(pFunc);
    }

} // end of namespace Tests
} // end of namespace Async

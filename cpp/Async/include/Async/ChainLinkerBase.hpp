#pragma once

#ifndef ASYNC_ICHAINLINKER_HPP
#define ASYNC_ICHAINLINKER_HPP

// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES
// #include "Async/IWorkItem.hpp"
#include "Async/Result.hpp"

// project namespace
namespace Async
{

    // a ChainLinker instance is used to implement the successor-parent Continuable
    // WorkItem relationship. The issue relies with templated Promise objects.
    // Promises are only templated with THEIR own information, so when creating
    // successor Promises, there is a template information issue regarding
    // HOW the successor will be stored. If the Promise stores successor
    // Promises, then a Promise instance needs to be templated with the successor
    // information...leading to an infinite sequence of template requirements.

    // So, this interface will be derived into classes that store successor
    // and parent Promise instances. This resolves the storing issue, as well
    // as solves some continuation semantics.
	class ChainLinkerBase
	{
	protected:

        // internal method to populate the successor.
		virtual Types::Result_t ApplyFunctionToChild() = 0;
	public:

		virtual ~ChainLinkerBase() = default;

        // will actually populate the successor.
		virtual Types::Result_t Chain() = 0;

	};

    // alias for shared pointer to a ChainLinker instance.
	using ChainLinkerBasePtr = std::shared_ptr<ChainLinkerBase>;

} // end of namespace Async

#endif // end of header guard

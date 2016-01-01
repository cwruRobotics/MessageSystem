#pragma once

#ifndef FUTURESFRAMEWORK_ICHAINLINKER_HPP
#define FUTURESFRAMEWORK_ICHAINLINKER_HPP

// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES
// #include "FuturesFramework/IWorkItem.hpp"
#include "FuturesFramework/Result.hpp"

// project namespace
namespace FuturesFramework
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
	class IChainLinker
	{
	protected:

        // internal method to populate the successor.
		virtual Types::Result_t ApplyFunctionToChild() = 0;
	public:

		virtual ~IChainLinker() = default;

        // will actually populate the successor.
		virtual Types::Result_t Chain() = 0;

	};

    // alias for shared pointer to a ChainLinker instance.
	using IChainLinkerPtr = std::shared_ptr<IChainLinker>;

} // end of namespace FuturesFramework

#endif // end of header guard

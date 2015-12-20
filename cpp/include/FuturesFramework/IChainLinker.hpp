#pragma once

#ifndef FUTURESFRAMEWORK_ICHAINLINKER_HPP
#define FUTURESFRAMEWORK_ICHAINLINKER_HPP

// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES
// #include "FuturesFramework/IWorkItem.hpp"
#include "FuturesFramework/Result.hpp"

namespace FuturesFramework
{

	class IChainLinker
	{
	protected:

		virtual Types::Result_t ApplyFunctionToChild() = 0;
	public:

		virtual ~IChainLinker() = default;

		virtual Types::Result_t Chain() = 0;

	};

	using IChainLinkerPtr = std::shared_ptr<IChainLinker>;

}

#endif

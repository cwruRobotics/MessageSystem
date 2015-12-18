#pragma once

#ifndef FUTURESFRAMEWORK_IEXECUTABLE_HPP
#define FUTURESFRAMEWORK_IEXECUTABLE_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/Result.hpp"

namespace FuturesFramework
{

	class IExecutable
	{
	protected:

		virtual Types::Result_t Execute() = 0;

		virtual Types::Result_t GetExecutionResult() = 0;
	};

}



#endif

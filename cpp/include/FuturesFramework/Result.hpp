#pragma once

#ifndef FUTURESFRAMEWORK_TYPES_RESULT_T_HPP
#define FUTURESFRAMEWORK_TYPES_RESULT_T_HPP

// SYSTEM INCLUDES
#include <string>

// C++ PROJECT INCLUDES


namespace FuturesFramework
{
namespace Types
{
	enum class Result_t
	{
		SUCCESS = 0,
		FAILURE = 1,
		UNKNOWN = 2,
	};
} // end of namespace Types

const std::string ResultToString(Types::Result_t result);

}

#endif
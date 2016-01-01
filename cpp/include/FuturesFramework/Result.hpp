#pragma once

#ifndef FUTURESFRAMEWORK_TYPES_RESULT_T_HPP
#define FUTURESFRAMEWORK_TYPES_RESULT_T_HPP

// SYSTEM INCLUDES
#include <string>

// C++ PROJECT INCLUDES

// project namespace
namespace FuturesFramework
{
// component namespace
namespace Types
{

    // used as the status of functions used internally
    // to the framework.
	enum class Result_t
	{
		SUCCESS = 0,
		FAILURE = 1,
		UNKNOWN = 2,
	};

} // end of namespace Types

// function to get the string representation of a Types::Result_t
const std::string ResultToString(Types::Result_t result);

} // end of namespace FuturesFramework

#endif // end of header guard

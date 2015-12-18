#pragma once

#ifndef FUTURESFRAMEWORK_STATES_PROMISESTATES_HPP
#define FUTURESFRAMEWORK_STATES_PROMISESTATES_HPP

// SYSTEM INCLUDES
#include <stdexcept>
#include <string>

// C++ PROJECT INCLUDES
// (none)

namespace FuturesFramework
{
namespace States
{

	enum class PromiseState
	{
		UNTOUCHED = 0,
		PENDING = 1,
		SUCCESS = 2,
		FAILURE = 3,
	};

} // end of namespace States

std::string GetPromiseStateString(States::PromiseState state);

} // end of namespace FuturesFramework

#endif // end of header guard

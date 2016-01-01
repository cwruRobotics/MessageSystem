#pragma once

#ifndef FUTURESFRAMEWORK_SETTLEMENTSTATES_HPP
#define FUTURESFRAMEWORK_SETTLEMENTSTATES_HPP

// SYSTEM INCLUDES
#include <string>

// C++ PROJECT INCLUDES

// project namespace
namespace FuturesFramework
{
// component namespace
namespace States
{

    // represents how a Promise is resolved.
    enum class SettlementState
    {
        PENDING = 0, // Promise not resolved yet.
        SUCCESS = 1, // Promise was resolved successfully. Contains results.
        FAILURE = 2, // Promise was resolved unsuccessfully. Contains error.
    };

} // end of namespace States

// Get the string representation of a States::SettlementState
std::string GetSettlementStateString(States::SettlementState state);

} // end of namespace FuturesFramework

#endif // end of header guard

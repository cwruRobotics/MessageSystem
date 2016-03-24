#pragma once

#ifndef ASYNC_SETTLEMENTSTATES_HPP
#define ASYNC_SETTLEMENTSTATES_HPP

// SYSTEM INCLUDES
#include <string>

// C++ PROJECT INCLUDES

// project namespace
namespace Async
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

} // end of namespace Async

#endif // end of header guard

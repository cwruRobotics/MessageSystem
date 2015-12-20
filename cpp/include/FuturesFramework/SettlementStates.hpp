#pragma once

#ifndef FUTURESFRAMEWORK_SETTLEMENTSTATES_HPP
#define FUTURESFRAMEWORK_SETTLEMENTSTATES_HPP

// SYSTEM INCLUDES
#include <string>

// C++ PROJECT INCLUDES

namespace FuturesFramework
{
namespace States
{
    enum class SettlementState
    {
        PENDING = 0,
        SUCCESS = 1,
        FAILURE = 2,
    };
}
std::string GetSettlementStateString(States::SettlementState state);
}


#endif

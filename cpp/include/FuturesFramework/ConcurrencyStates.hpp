#pragma once

#ifndef FUTURESFRAMEWORK_STATES_CONCURRENCYSTATES_HPP
#define FUTURESFRAMEWORK_STATES_CONCURRENCYSTATES_HPP

// SYSTEM INCLUDES
#include <string>

// C++ PROJECT INCLUDES


namespace FuturesFramework
{
namespace States
{

    enum class ConcurrencyState
    {
        IDLE = 0,
        BUSY = 1,
        DONE = 2,
    };

} // end of namespace States

std::string GetConcurrencyStateString(const States::ConcurrencyState& state);

} // end of namespace FuturesFramework

#endif

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/ConcurrencyStates.hpp"

namespace Async
{

    std::string GetConcurrencyStateString(const States::ConcurrencyState& state)
    {
        switch(state)
        {
            case States::ConcurrencyState::IDLE:
                return "IDLE";
            case States::ConcurrencyState::BUSY:
                return "BUSY";
            case States::ConcurrencyState::DONE:
                return "DONE";
            throw std::exception("Unknown State");
        }
        return "";
    }

} // end of namespace Async

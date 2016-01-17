// SYSTEM INCLUDES
#include <stdexcept>

// C++ PROJECT INCLUDES
#include "Async/SettlementStates.hpp"

namespace Async
{
    std::string GetSettlementStateString(States::SettlementState state)
    {
        switch(state)
        {
            case States::SettlementState::PENDING:
                return "PENDING";
            case States::SettlementState::SUCCESS:
                return "SUCCESS";
            case States::SettlementState::FAILURE:
                return "FAILURE";
            throw std::logic_error("Unknown State");
        }
        return "";
    }
}

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "FuturesFramework/PromiseStates.hpp"

namespace FuturesFramework
{

	std::string GetPromiseStateString(States::PromiseState state)
	{
		switch (state)
		{
		case States::PromiseState::UNTOUCHED:
			return "UNTOUCHED";
		case States::PromiseState::PENDING:
			return "PENDING";
		case States::PromiseState::SUCCESS:
			return "SUCCESS";
		case States::PromiseState::FAILURE:
			return "FAILURE";
		}
		throw std::logic_error("Unexpected Promise State");
	}

}

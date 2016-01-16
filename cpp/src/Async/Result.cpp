// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/Result.hpp"

namespace Async
{
	const std::string ResultToString(Types::Result_t result)
	{
		switch (result)
		{
		case Types::Result_t::SUCCESS:
			return "SUCCESS";
		case Types::Result_t::FAILURE:
			return "FAILURE";
		case Types::Result_t::UNKNOWN:
			return "UNKNOWN";
		default:
			throw std::exception("Unknown Result type");
		}
	}
}

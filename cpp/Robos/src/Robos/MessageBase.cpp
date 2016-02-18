// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Robos/MessageBase.hpp"

namespace Robos
{

    MessageBase::MessageBase(std::string s) : topic(s)
    {
    }

    MessageBase::~MessageBase()
    {
    }

}

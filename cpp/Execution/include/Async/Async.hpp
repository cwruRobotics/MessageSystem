#pragma once
#ifndef ASYNC_ASYNC_HPP
#define ASYNC_ASYNC_HPP

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/LibraryExport.hpp"

namespace Async
{
    ASYNC_API bool Start(std::string configPath);

    ASYNC_API bool Stop();
}

#endif // end of ASYNC_ASYNC_HPP

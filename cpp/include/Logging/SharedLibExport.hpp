#pragma once

// SYSTEM INCLUDES
#include <iostream>

// C++ PROJECT INCLUDES

#if defined(__cplusplus)
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__

// defined by CMake when building this project as
// a static library
#ifdef LOGGING_STATIC_DEFINE
#   define LOGGING_API
#   define LOGGING_LOCAL

#else
    // protection
#   ifndef LOGGING_API
#       ifdef Logging_EXPORTS  // defined by CMake
// We are building this library
#           ifdef __GNUC__
#               define LOGGING_API  __attribute__((dllexport))
#           else
#               define LOGGING_API  __declspec(dllexport)
#           endif
#       else
// We are using this library
#           ifdef __GNUC__
#               define LOGGING_API  __attribute__((dllimport))
#           else
#               define LOGGING_API  __declspec(dllimport)
#           endif
#       endif
#   endif
#   ifndef LOGGING_LOCAL
#       define LOGGING_LOCAL
#   endif
#endif

#else  // Not a _WIN32 platform. Use gcc 'visibility' attribute.

#   if __GNUC__ >= 4
#       define LOGGING_API              __attribute__((visibility("default")))
#       define LOGGING_LOCAL            __attribute__((visibility("hidden")))
#   else
#       define LOGGING_API
#       define LOGGING_LOCAL
#   endif

#endif  // _WIN32

#if defined(__cplusplus)
}
#endif
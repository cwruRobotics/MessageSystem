#pragma once

// SYSTEM INCLUDES
#include <iostream>

// C++ PROJECT INCLUDES

#if defined(__cplusplus)
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__ || _WIN64

// defined by CMake when building this project as
// a static library
#ifdef FUTURESFRAMEWORK_STATIC_DEFINE
#   define FUTURESFRAMEWORK_API
#   define FUTURESFRAMEWORK_API_LOCAL

#else
    // protection
#   ifndef FUTURESFRAMEWORK_API
#       ifdef FuturesFramework_EXPORTS  // defined by CMake
// We are building this library
#           ifdef __GNUC__
#               define FUTURESFRAMEWORK_API  __attribute__((dllexport))
#           else
#               define FUTURESFRAMEWORK_API  __declspec(dllexport)
#           endif
#       else
// We are using this library
#           ifdef __GNUC__
#               define FUTURESFRAMEWORK_API  __attribute__((dllimport))
#           else
#               define FUTURESFRAMEWORK_API  __declspec(dllimport)
#           endif
#       endif
#   endif
#   ifndef FUTURESFRAMEWORK_LOCAL
#       define FUTURESFRAMEWORK_LOCAL
#   endif
#endif

#else  // Not a _WIN32 platform. Use gcc 'visibility' attribute.

#   if __GNUC__ >= 4
#       define FUTURESFRAMEWORK_API              __attribute__((visibility("default")))
#       define FUTURESFRAMEWORK_LOCAL            __attribute__((visibility("hidden")))
#   else
#       define FUTURESFRAMEWORK_API
#       define FUTURESFRAMEWORK_LOCAL
#   endif

#endif  // _WIN32

#if defined(__cplusplus)
}
#endif

#pragma once

// SYSTEM INCLUDES

// C++ PROJECT INCLUDES

// we need to declare our code "C" external so that outside C++ or C
// code can use it.
#if defined(__cplusplus)
extern "C" {
#endif

// windows platform
#if defined _WIN32 || defined __CYGWIN__ || _WIN64

// defined by CMake when building this project as
// a static library or an executable
#ifdef LOGGING_STATIC_DEFINE
#   define LOGGING_API
#   define LOGGING_LOCAL

// importing or exporting this project.
#else
    // protection (similar to a header guard)
#   ifndef LOGGING_API
#       ifdef Logging_EXPORTS  // defined by CMake

            // We are building this library
            // syntax depending on compiler being used.
#           ifdef __GNUC__ // GNU compiler.
#               define LOGGING_API  __attribute__((dllexport))

            // can clang be used here?
            // insert clang case here.

#           else // Visual Studio compiler (on windows assumed if not GNU then CL)
#               define LOGGING_API  __declspec(dllexport)
#           endif
#       else

            // We are using this library
            // syntax depending on compiler being used.
#           ifdef __GNUC__ // GNU compiler
#               define LOGGING_API  __attribute__((dllimport))

            // can clang be used here?
            // insert clang case here.

#           else // Visual Studio compiler (on windows assumed if not GNU then CL)
#               define LOGGING_API  __declspec(dllimport)
#           endif
#       endif
#   endif
    // protection (like a header guard)
#   ifndef LOGGING_LOCAL
#       define LOGGING_LOCAL
#   endif
#endif

#else  // Not a _WIN32 platform. Use gcc 'visibility' attribute.

#   if __GNUC__ >= 4 // visibility is only supported in GNU versions >= 4
#       define LOGGING_API              __attribute__((visibility("default")))
#       define LOGGING_LOCAL            __attribute__((visibility("hidden")))
#   else
#       define LOGGING_API
#       define LOGGING_LOCAL
#   endif

#endif  // _WIN32 || __CYGWIN__ || _WIN64

#if defined(__cplusplus)
}
#endif

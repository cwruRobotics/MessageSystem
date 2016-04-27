#pragma once

// SYSTEM INCLUDES

// C++ PROJECT INCLUDES

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__ || _WIN64

// defined by CMake when building this project as
// a static library
#ifdef TUTORIALROBOT_GPS_STATIC_DEFINE
#   define TUTORIALROBOT_GPS_API
#   define TUTORIALROBOT_GPS_LOCAL

#else
    
#   ifndef TUTORIALROBOT_GPS_API

        // this variable is defined by CMake
        // in the instruction(include(GenerateExportHeaders)).
        // if it is defined, then this project is being built
#       ifdef TutorialRobot_GPS_EXPORTS

            // determine what compiler is being run. We could be
            // using Cygwin and running GCC. If so, then this
            // is the syntax to declare a project to be exported
            // with GCC.
#           ifdef __GNUC__
#               define TUTORIALROBOT_GPS_API  __attribute__((dllexport))

            // otherwise, assume we are using CodeBlocks or Visual Studio.
            // This is the syntax for declaring a project to be exported
            // with CodeBlocks and Visual Studio.
#           else
#               define TUTORIALROBOT_GPS_API  __declspec(dllexport)
#           endif

        // if this variable is not defined, then CMake is not being
        // run and we are importing this project.
#       else

            // same Compiler detection and syntax
#           ifdef __GNUC__
#               define TUTORIALROBOT_GPS_API  __attribute__((dllimport))
#           else
#               define TUTORIALROBOT_GPS_API  __declspec(dllimport)
#           endif
#       endif
#   endif

    // this variable is only used in the Case of GCC being used
    // on a pure unix distribution. It does not mean anything
    // otherwise, but must be defined to prevent a compilation
    // error for non unix builds.
#   ifndef TUTORIALROBOT_GPS_LOCAL
#       define TUTORIALROBOT_GPS_LOCAL
#   endif
#endif

// _WIN32, __CYGWIN__, and _WIN64 are not defined. This is for a unix
// based OS build. We will assume GCC is the compiler being used
#else

    // visibility attributes are only supported for GCC >= 4 versions.
#   if __GNUC__ >= 4
#       define TUTORIALROBOT_GPS_API              __attribute__((visibility("default")))
#       define TUTORIALROBOT_GPS_LOCAL            __attribute__((visibility("hidden")))
#   else
#       define TUTORIALROBOT_GPS_API
#       define TUTORIALROBOT_GPS_LOCAL
#   endif

#endif  // _WIN32, __CYGWIN__, _WIN64

#ifdef __cplusplus
}
#endif

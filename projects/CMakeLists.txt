# ===============================================================================
# CMAKE PROPERTIES
# ===============================================================================

# this is the required version of CMake necessary
# to run. CMake will fail if you have anything
# less than 3.3
cmake_minimum_required( VERSION 3.3 )

# when CMake generates Makefiles, this setting
# will allow them to print to the console.
set( CMAKE_VERBOSE_MAKEFILE "ON" )

# this points to the root of the cmake directory
# which contains all the toolchains and modules
get_filename_component( CMAKE_LOCAL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../cmake ABSOLUTE )

# add our custom module path to the CMAKE_MODULE_PATH
# this allows us to say include( <module_name> )
# if <module_name> is located in ${CMAKE_LOCAL_ROOT}/modules
list( APPEND CMAKE_MODULE_PATH ${CMAKE_LOCAL_ROOT}/modules )

# ===============================================================================
# PROJECT PROPERTIES
# ===============================================================================

# this is the project name and type
project( Robos CXX )

get_filename_component( SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../cpp ABSOLUTE )

# this is where all build intermediate files will go
get_filename_component( OUT_ROOT ${CMAKE_CURRENT_BINARY_DIR}/out ABSOLUTE )

# ===============================================================================
# PROJECT EXTERNAL OPTIONS
# ===============================================================================

option( RUN_UNIT_TESTS "Unit test support (default OFF)" OFF )
option( RUN_CPP_CHECK  "Static analysis support (default OFF)" OFF )

# ===============================================================================
# PROJECT SUBMODULES
# ===============================================================================

# documentation variables...ignore for now
set(DOC_PUBLIC_INPUT_LIST )
set(DOC_PRIVATE_INPUT_LIST )

# these variables are set through the python build scripts.
# they are what controls the project build flow.
if( BUILD_LOGGING )
    # Logger library
    add_subdirectory( ${SRC_ROOT}/Logging/src/Logging
                      ${OUT_ROOT}/Logging )

    # give other subprojects a chance to search for libraries and header
    # files created before the "make install"
    list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/Logging" )
    list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/Logging" )
endif()

if( BUILD_GTEST )
    # gtest library
    add_subdirectory( ${SRC_ROOT}/TestingFrameworks/gtest/src
                      ${OUT_ROOT}/gtest )

    # give other subprojects a chance to search for libraries and header
    # files created before the "make install"
    list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/gtest" )
    list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/gtest" )
endif()

if( BUILD_UTILITIES )
    # Utilities library
    add_subdirectory( ${SRC_ROOT}/Utilities/src/Utilities
                      ${OUT_ROOT}/Utilities )

    # give other subprojects a chance to search for libraries and header
    # files created before the "make install"
    list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/Utilities" )
    list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/Utilities" )
endif()

if( BUILD_UTILITIES_FT )
    # Utilities_FT executable
    add_subdirectory( ${SRC_ROOT}/Utilities_FT/src
                      ${OUT_ROOT}/Utilities_FT )

    # give other subprojects a chance to search for libraries and header
    # files created before the "make install"
    list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/Utilities_FT" )
    list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/Utilities_FT" )
endif()

if( BUILD_ASYNC )
    # Async library
    add_subdirectory( ${SRC_ROOT}/Async/src/Async
                      ${OUT_ROOT}/Async )

    # give other subprojects a chance to search for libraries and header
    # files created before the "make install"
    list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/Async" )
    list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/Async" )
endif()

if( BUILD_ASYNC_FT )
    # Async_FT executable
    add_subdirectory( ${SRC_ROOT}/Async_FT/src
                      ${OUT_ROOT}/Async_FT )

    # give other subprojects a chance to search for libraries and header
    # files created before the "make install"
    list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/Async_FT" )
    list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/Async_FT" )
endif()

if( BUILD_ROBOS )
    # Robos library
    add_subdirectory( ${SRC_ROOT}/Robos/src/Robos
                      ${OUT_ROOT}/Robos )

    # give other subprojects a chance to search for libraries and header
    # files created before the "make install"
    list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/Robos" )
    list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/Robos" )
endif()

if( BUILD_ROBOS_FT )
    # Async_FT executable
    add_subdirectory( ${SRC_ROOT}/Robos_FT/src
                      ${OUT_ROOT}/Robos_FT )

    # give other subprojects a chance to search for libraries and header
    # files created before the "make install"
    list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/Robos_FT" )
    list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/Robos_FT" )
endif()

if( BUILD_TUTORIALROBOT )
    # Async_FT executable
    add_subdirectory( ${SRC_ROOT}/TutorialRobot/src
                      ${OUT_ROOT}/TutorialRobot )

    # give other subprojects a chance to search for libraries and header
    # files created before the "make install"
    list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/TutorialRobot" )
    list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/TutorialRobot" )
endif()

# this variable is also set through the python build scripts.
if( RUN_UNIT_TESTS )

    if( BUILD_LOGGING )
        # Logger unit tests
        add_subdirectory( ${SRC_ROOT}/Logging/src/Logging/unitTest
                          ${OUT_ROOT}/LoggerUnit )
    endif()

    if( BUILD_UTILITIES )
        # Utilities unit tests
        add_subdirectory( ${SRC_ROOT}/Utilities/src/Utilities/unitTest
                          ${OUT_ROOT}/UtilitiesUnit )
    endif()

    if( BUILD_ASYNC )
        # Async unit tests
        add_subdirectory( ${SRC_ROOT}/Async/src/Async/unitTest
                          ${OUT_ROOT}/AsyncUnit )
    endif()

    if( BUILD_ROBOS )
        # Robos unit tests
        add_subdirectory( ${SRC_ROOT}/Robos/src/Robos/unitTest
                          ${OUT_ROOT}/RobosUnit )
    endif()

endif()


# if( RUN_CPP_CHECK )
#     find_package( cppcheck REQUIRED)
#     if( CPPCHECK_FOUND )
#          # include the .cmake file for CppCheck to run
#          # if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
#               # add windows.cfg (set ( CPPCHECK_LIBRARY_USE "--library=windows.cfg"))
#          # use linux version
#
#        add_custom_target(TARGET
#            # cppcheck target name
#            PRE_BUILD
#            COMMAND ${CMAKE_COMMAND} -E make_directory # output directory
#        )
#     else()
#         message(ERROR "CppCheck not found")
#     endif()
# endif()
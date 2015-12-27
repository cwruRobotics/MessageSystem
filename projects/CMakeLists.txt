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

list( APPEND CMAKE_MODULE_PATH ${CMAKE_LOCAL_ROOT}/modules )

# ===============================================================================
# PROJECT PROPERTIES
# ===============================================================================

# this is the project name and type
project( MessageFramework CXX )

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

set(DOC_PUBLIC_INPUT_LIST )
set(DOC_PRIVATE_INPUT_LIST )

if( BUILD_LOGGING )
    # Logger library
    add_subdirectory( ${SRC_ROOT}/src/Logging
                      ${OUT_ROOT}/Logging )

    # give other subprojects a chance to search for libraries and header
    # files created before the "make install"
    list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/Logging" )
    list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/Logging" )
endif()

if( BUILD_FUTURESFRAMEWORK )
    # FuturesFramework library
    add_subdirectory( ${SRC_ROOT}/src/FuturesFramework
                      ${OUT_ROOT}/FuturesFramework )

    # give other subprojects a chance to search for libraries and header
    # files created before the "make install"
    list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/FuturesFramework" )
    list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/FuturesFramework" )
endif()

if( BUILD_MESSAGEFRAMEWORK )
    # MessageFramework library
    add_subdirectory( ${SRC_ROOT}/src/MessageFramework
                      ${OUT_ROOT}/MessageFramework )

    # give other subprojects a chance to search for libraries and header
    # files created before the "make install"
    list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/MessageFramework" )
    list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/MessageFramework" )
endif()

if( RUN_UNIT_TESTS )

    if( BUILD_LOGGING )
        # Logger unit tests
        add_subdirectory( ${SRC_ROOT}/src/Logging/unitTest
                          ${OUT_ROOT}/LoggerUnit )
    endif()

    if( BUILD_FUTURESFRAMEWORK )
        # FuturesFramework unit tests
        # add_subdirectory( ${SRC_ROOT}/src/FuturesFramework/unittest
        #                   ${OUT_ROOT}/LoggerUnit )
    endif()

    if( BUILD_MESSAGEFRAMEWORK )
        # MessageFramework unit tests
        # add_subdirectory( ${SRC_ROOT}/src/MessageFramework/unittest
        #                   ${OUT_ROOT}/LoggerUnit )
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
#     add_custom_target(TARGET
#          # cppcheck target name
#          PRE_BUILD
#          COMMAND ${CMAKE_COMMAND} -E make_directory # output directory
#     )
#     else()
#         message(ERROR "CppCheck not found")
#     endif()
# endif()
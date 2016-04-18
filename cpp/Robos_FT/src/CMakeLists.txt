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
get_filename_component( CMAKE_LOCAL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake ABSOLUTE )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_LOCAL_ROOT}/modules )
# ===============================================================================
# PROJECT PROPERTIES
# ===============================================================================

# Project name
project( Robos_FT CXX )

# source code root directory
get_filename_component( SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE )

# Public header file root directory
get_filename_component( INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../include ABSOLUTE )

get_filename_component( CONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/out/include ABSOLUTE )

# get_filename_component( TEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../TestingFrameworks ABSOLUTE )

if( PYTHON_ENABLED )
    set( BUILD_ROBOS_PYTHON_FT_CLIENT TRUE )
endif()

if( BUILD_ROBOS_CPP_FT_CLIENT OR (NOT BUILD_ROBOS_PYTHON_FT_CLIENT AND NOT BUILD_ROBOS_JAVA_FT_CLIENT) )
    message("Building Cpp Client")
    # Logger library
    add_subdirectory( ${SRC_ROOT}/../Robos_FT_Cpp_Client/src
                      ${OUT_ROOT}/Robos_FT_Cpp_Client )

    # give other subprojects a chance to search for libraries and header
    # files created before the "make install"
    list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/Robos_FT_Cpp_Client" )
    list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/Robos_FT_Cpp_Client" )
endif()
if( BUILD_ROBOS_PYTHON_FT_CLIENT )
    # Logger library
    add_subdirectory( ${SRC_ROOT}/../Robos_FT_Python_Client/src
                      ${OUT_ROOT}/Robos_FT_Python_Client )

    # give other subprojects a chance to search for libraries and header
    # files created before the "make install"
    list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/Robos_FT_Python_Client" )
    list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/Robos_FT_Python_Client" )
endif()
if( BUILD_ROBOS_JAVA_FT_CLIENT )
    # Logger library
    add_subdirectory( ${SRC_ROOT}/../Robos_FT_Java_Client/src
                      ${OUT_ROOT}/Robos_FT_Java_Client )

    # give other subprojects a chance to search for libraries and header
    # files created before the "make install"
    list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/Robos_FT_Java_Client" )
    list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/Robos_FT_Java_Client" )
endif()

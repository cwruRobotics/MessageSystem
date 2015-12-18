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
project( FuturesFramework CXX )

# source code root directory
get_filename_component( SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/.. ABSOLUTE )

# Public header file root directory
get_filename_component( INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../include ABSOLUTE )

# ===============================================================================
# PROJECT EXTERNAL OPTIONS
# ===============================================================================
# this is where we could get the Version numbering from Version.h and
# save them as CMake variables. for now, lets just declare them
set( PROJECT_VERSION_MAJOR "1" )
set( PROJECT_VERSION_MINOR "0" )
set( PROJECT_VERSION_PATCH "0" )

# -------------------------------------------------------------------------------
# Logging library

# Source files
set( FUTURESFRAMEWORK_SRCS
    ${SRC_ROOT}/FuturesFramework/Result.cpp
    ${SRC_ROOT}/FuturesFramework/WorkItemStates.cpp
)

# Public header files (installed together with libraries)
set( FUTURESFRAMEWORK_PUBLIC_HEADERS
    ${INC_ROOT}/FuturesFramework/SharedLibExport.hpp
    ${INC_ROOT}/FuturesFramework/Result.hpp
    ${INC_ROOT}/FuturesFramework/ScheduleRequestStates.hpp
    ${INC_ROOT}/FuturesFramework/WorkItemStates.hpp
    ${INC_ROOT}/FuturesFramework/IWorkItem.hpp
)

# setup visual studio source groups
source_group( PublicHeaders FILES ${FUTURESFRAMEWORK_PUBLIC_HEADERS} )

# Directories to search for header files
set( INCLUDE_DIRS
    ${INC_ROOT}
    ${SRC_ROOT}
)

# load external modules
# (none yet)...Logging does not depend on anything else

# set target specific compiler definitions
# this should ONLY be definitions needed to build the source code.
# examples: macros ON/OFF, etc.
set( FUTURESFRAMEWORK_COMPILE_DEFINITIONS )

# check required libraries and header files and such.
# this will fail CMake if set to False due to required
# modules requiring their own dependencies
set( DEPENDENCY_CHECK TRUE )

# OS specific files and definitions
# if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )

#     include( ${SRC_ROOT}/Logging/Windows/Windows.cmake )
#     include( GNUInstallDirs ) # TODO make a Windows module if necessary
    
# elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    
#     include( ${SRC_ROOT}/Logging/Linux/Linux.cmake )
#     include( GNUInstallDirs ) # TODO make a Windows module if necessary

# elseif( ${CMAKE_SYSTEM_NAME} MATCHES "OSX" )

#     include( ${SRC_ROOT}/Logging/OSX/OSX.cmake )
#     include( GNUInstallDirs ) # TODO make a Windows module if necessary

# else()
#     message( SEND_ERROR "${CMAKE_SYSTEM_NAME} system build not supported" )
# endif()

include( GNUInstallDirs )

if( NOT DEPENDENCY_CHECK )
    message( SEND_ERROR "Required header files or libraries not found" )
endif()


# -------------------------------------------------------------------------------
# TARGET SETUP
# -------------------------------------------------------------------------------

include_directories( ${INCLUDE_DIRS} )

# -------------------------------------------------------------------------------
# Libraries
#

# Logging library
set( FUTURESFRAMEWORK_LIB_NAME ${PROJECT_NAME} )

# Shared Library export header file supporte
include( GenerateExportHeader )

add_library( ${FUTURESFRAMEWORK_LIB_NAME} SHARED ${FUTURESFRAMEWORK_SRCS} ${FUTURESFRAMEWORK_PUBLIC_HEADERS} )
# target_link_libraries( ${FUTURESFRAMEWORK_LIB_NAME} ${FUTURESFRAMEWORK_LIBS} )

set_target_properties( ${FUTURESFRAMEWORK_LIB_NAME} PROPERTIES
    PUBLIC_HEADER       "${FUTURESFRAMEWORK_PUBLIC_HEADERS}"
    COMPILE_DEFINITIONS "${FUTURESFRAMEWORK_COMPILE_DEFINITIONS}"
    SOVERSION           "${PROJECT_VERSION_MAJOR}"
    VERSION             "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

# -------------------------------------------------------------------------------
# INSTALLATION
# -------------------------------------------------------------------------------

# install targets
install( TARGETS ${FUTURESFRAMEWORK_LIB_NAME}
         RUNTIME        DESTINATION ${CMAKE_INSTALL_BINDIR}
         LIBRARY        DESTINATION ${CMAKE_INSTALL_LIBDIR}
         ARCHIVE        DESTINATION ${CMAKE_INSTALL_LIBDIR}
         PUBLIC_HEADER  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/FuturesFramework
)

if( MSVC )
    get_filename_component( PDB_DIR ${OUT_ROOT}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ABSOLUTE )
    install( DIRECTORY ${PDB_DIR}
        DESTINATION .
        FILES_MATCHING
        PATTERN ${FUTURESFRAMEWORK_LIB_NAME}.pdb
    )
endif()

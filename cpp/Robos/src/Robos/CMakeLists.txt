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
project( MessageFramework CXX )

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

# You will add your C++ files here.
# There are three source groups (you can (make more)/(delete some) if necessary)
# We are telling CMake that these are how our C++ files will be distributed
# in our project. All C++ Source files (.cpp files) MUST go in the
# MESSAGEFRAMEWORK_SRCS list. The compiler will try to compile all sources
# from this list. MESSAGEFRAMEWORK_PUBLIC_HEADERS should contain all
# external headers (headers that anyone who uses this library should)
# have access to.
# The MESSAGEFRAMEWORK_PRIVATE_HEADERS list is all header files that are
# meant to be internal to the project. Use ${SRC_ROOT} and ${INC_ROOT}
# variables to give CMake the paths to your files (${SRC_ROOT} is for .cpp files,
# ${INC_ROOT} is for all .hpp files).

# Remember, ${INC_ROOT} was set at the beginning of this CMakeLists.txt
# and points to the top level "include" directory. You will need to
# specify the Folder that your files are in, (same for ${SRC_ROOT}).

# Source files
set( MESSAGEFRAMEWORK_SRCS
# EX:   ${SRC_ROOT}/MessageFramework/MySourceFile.cpp
)

# Public header files (installed together with libraries)
set( MESSAGEFRAMEWORK_PUBLIC_HEADERS
# EX:   ${INC_ROOT}/MessageFramework/MyHeaderFileIWantClientsToHaveAccessTo.hpp
)

set( MESSAGEFRAMEWORK_PRIVATE_HEADERS
# EX:   ${INC_ROOT}/MessageFramework/MyInternalHeaderFile.hpp
)

# setup visual studio source groups
source_group( PublicHeaders FILES ${MESSAGEFRAMEWORK_PUBLIC_HEADERS} )

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
set( MESSAGEFRAMEWORK_COMPILE_DEFINITIONS )

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
set( MESSAGEFRAMEWORK_LIB_NAME ${PROJECT_NAME} )

# Shared Library export header file supporte
include( GenerateExportHeader )

add_library( ${MESSAGEFRAMEWORK_LIB_NAME} SHARED ${MESSAGEFRAMEWORK_SRCS} ${MESSAGEFRAMEWORK_PUBLIC_HEADERS} ${MESSAGEFRAMEWORK_PRIVATE_HEADERS})

# this command will link our project to all libraries that it depends on (like FuturesFramework and Logging).
# I haven't figured out the process of linking yet in CMake so this is commented out for now.
# target_link_libraries( ${FUTURESFRAMEWORK_LIB_NAME} ${FUTURESFRAMEWORK_LIBS} )

set_target_properties( ${MESSAGEFRAMEWORK_LIB_NAME} PROPERTIES
    PUBLIC_HEADER       "${MESSAGEFRAMEWORK_PUBLIC_HEADERS}"
    COMPILE_DEFINITIONS "${MESSAGEFRAMEWORK_COMPILE_DEFINITIONS}"
    SOVERSION           "${PROJECT_VERSION_MAJOR}"
    VERSION             "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

# -------------------------------------------------------------------------------
# INSTALLATION
# -------------------------------------------------------------------------------

# install targets
install( TARGETS ${MESSAGEFRAMEWORK_LIB_NAME}
         RUNTIME        DESTINATION ${CMAKE_INSTALL_BINDIR}
         LIBRARY        DESTINATION ${CMAKE_INSTALL_LIBDIR}
         ARCHIVE        DESTINATION ${CMAKE_INSTALL_LIBDIR}
         PUBLIC_HEADER  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MessageFramework
)

if( MSVC )
    get_filename_component( PDB_DIR ${OUT_ROOT}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ABSOLUTE )
    install( DIRECTORY ${PDB_DIR}
        DESTINATION .
        FILES_MATCHING
        PATTERN ${MESSAGEFRAMEWORK_LIB_NAME}.pdb
    )
endif()

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
get_filename_component( CMAKE_LOCAL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../../cmake ABSOLUTE )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_LOCAL_ROOT}/modules )

# ===============================================================================
# PROJECT PROPERTIES
# ===============================================================================

# Project name
project( Async CXX )

# source code root directory
get_filename_component( SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/.. ABSOLUTE )

# Public header file root directory
get_filename_component( INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../include ABSOLUTE )

get_filename_component( CONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../../build/out/include ABSOLUTE )

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
set( ASYNC_SRCS
    # logging
    ${CONFIG_ROOT}/AsyncLoggingConfig.cpp

    ${SRC_ROOT}/Async/Result.cpp
    ${SRC_ROOT}/Async/WorkItemStates.cpp
    ${SRC_ROOT}/Async/WorkItemStateMachine.cpp
    ${SRC_ROOT}/Async/Scheduler.cpp
    ${SRC_ROOT}/Async/WorkItem.cpp
    ${SRC_ROOT}/Async/SettlementStates.cpp
    ${SRC_ROOT}/Async/ContinuableWorkItem.cpp
    ${SRC_ROOT}/Async/PromiseBase.cpp
    ${SRC_ROOT}/Async/WorkerThread.cpp
    ${SRC_ROOT}/Async/Engine.cpp

    # factories
    ${SRC_ROOT}/Async/Factories/EngineFactory.cpp
)

# Public header files (installed together with libraries)
set( ASYNC_PUBLIC_HEADERS
    ${INC_ROOT}/Async/LibraryExport.hpp
    ${INC_ROOT}/Async/Result.hpp
    ${INC_ROOT}/Async/SettlementStates.hpp
    ${INC_ROOT}/Async/IWorkItem.hpp
    ${INC_ROOT}/Async/IExecutableWorkItem.hpp
    ${INC_ROOT}/Async/IScheduler.hpp
    ${INC_ROOT}/Async/IChainLinker.hpp
    ${INC_ROOT}/Async/IContinuableWorkItem.hpp
    ${INC_ROOT}/Async/PromiseBase.hpp
    ${INC_ROOT}/Async/Promise.hpp
    ${INC_ROOT}/Async/IEngine.hpp
    ${INC_ROOT}/Async/AsyncExecution.hpp
)

set( ASYNC_PRIVATE_HEADERS
    # logging
    ${CONFIG_ROOT}/AsyncLoggingConfig.hpp

    ${INC_ROOT}/Async/WorkItemStates.hpp
    ${INC_ROOT}/Async/WorkItemStateMachine.hpp
    ${INC_ROOT}/Async/Scheduler.hpp
    ${INC_ROOT}/Async/WorkItem.hpp
    ${INC_ROOT}/Async/ContinuableWorkItem.hpp
    ${INC_ROOT}/Async/IThread.hpp
    ${INC_ROOT}/Async/WorkerThread.hpp
    ${INC_ROOT}/Async/Engine.hpp
    ${INC_ROOT}/Async/SimpleChainLinker.hpp
)

# setup visual studio source groups
source_group( PublicHeaders FILES ${ASYNC_PUBLIC_HEADERS} )

# Directories to search for header files
set( INCLUDE_DIRS
    ${INC_ROOT}
    ${SRC_ROOT}
    ${CONFIG_ROOT}
    ${CMAKE_PREFIX_PATH}/include
)

# load external modules
# (none yet)...Logging does not depend on anything else

# set target specific compiler definitions
# this should ONLY be definitions needed to build the source code.
# examples: macros ON/OFF, etc.
set( ASYNC_COMPILE_DEFINITIONS )

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

set( ASYNC_LIBS )
find_package( Logging REQUIRED )
if( LOGGING_FOUND )
    add_library( LOGGING SHARED IMPORTED )
    set_property( TARGET LOGGING PROPERTY IMPORTED_LOCATION ${LOGGING_SHARED} )
    set_property( TARGET LOGGING PROPERTY IMPORTED_IMPLIB ${LOGGING_LIBRARY} )
    # add_definitions(-DTESTMATHUTILS=1)

    list( APPEND ASYNC_LIBS LOGGING )

else()
    message( WARNING "Logging not found...skipping usage" )
endif()
list( LENGTH ASYNC_LIBS ASYNC_LIBS_LENGTH )

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
set( ASYNC_LIB_NAME ${PROJECT_NAME} )

# Shared Library export header file supporte
include( GenerateExportHeader )

add_library( ${ASYNC_LIB_NAME} SHARED ${ASYNC_SRCS} ${ASYNC_PUBLIC_HEADERS} ${ASYNC_PRIVATE_HEADERS})
if( ${ASYNC_LIBS_LENGTH} GREATER 0 )
    target_link_libraries( ${ASYNC_LIB_NAME} ${ASYNC_LIBS} )
endif()

set_target_properties( ${ASYNC_LIB_NAME} PROPERTIES
    PUBLIC_HEADER       "${ASYNC_PUBLIC_HEADERS}"
    COMPILE_DEFINITIONS "${ASYNC_COMPILE_DEFINITIONS}"
    SOVERSION           "${PROJECT_VERSION_MAJOR}"
    VERSION             "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

# -------------------------------------------------------------------------------
# INSTALLATION
# -------------------------------------------------------------------------------

# install targets
install( TARGETS ${ASYNC_LIB_NAME}
         RUNTIME        DESTINATION ${CMAKE_INSTALL_BINDIR}
         LIBRARY        DESTINATION ${CMAKE_INSTALL_LIBDIR}
         ARCHIVE        DESTINATION ${CMAKE_INSTALL_LIBDIR}
         PUBLIC_HEADER  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Async
)

if( MSVC )
    get_filename_component( PDB_DIR ${OUT_ROOT}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ABSOLUTE )
    install( DIRECTORY ${PDB_DIR}
        DESTINATION .
        FILES_MATCHING
        PATTERN ${ASYNC_LIB_NAME}.pdb
    )
endif()

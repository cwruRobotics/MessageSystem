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

# Project name
project( Async CXX )

# source code root directory
get_filename_component( SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/.. ABSOLUTE )

# Public header file root directory
get_filename_component( INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../include ABSOLUTE )

get_filename_component( CONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../../build/out/include ABSOLUTE )

# this is where we could get the Version numbering from Version.h and
# save them as CMake variables. for now, lets just declare them
set( PROJECT_VERSION_MAJOR "1" )
set( PROJECT_VERSION_MINOR "0" )
set( PROJECT_VERSION_PATCH "0" )

# Source files
set( ASYNC_SRCS
    # logging
    ${CONFIG_ROOT}/AsyncConfig.cpp

    ${SRC_ROOT}/Async/Result.cpp
    ${SRC_ROOT}/Async/WorkItemStates.cpp
    # ${SRC_ROOT}/Async/WorkItemStateMachine.cpp
    ${SRC_ROOT}/Async/Scheduler.cpp
    ${SRC_ROOT}/Async/WorkItem.cpp
    ${SRC_ROOT}/Async/SettlementStates.cpp
    # ${SRC_ROOT}/Async/ContinuableWorkItem.cpp
    ${SRC_ROOT}/Async/JobPriorities.cpp
    ${SRC_ROOT}/Async/PromiseBaseImpl.cpp
    ${SRC_ROOT}/Async/ConcurrencyStates.cpp
    ${SRC_ROOT}/Async/WorkerThread.cpp
    ${SRC_ROOT}/Async/Engine.cpp

    # factories
    ${SRC_ROOT}/Async/Factories/EngineFactory.cpp
)

# Public header files (installed together with libraries)
set( ASYNC_PUBLIC_HEADERS
    ${INC_ROOT}/Async/LibraryExport.hpp
    ${INC_ROOT}/Async/Result.hpp
    ${INC_ROOT}/Async/WorkItemStates.hpp
    ${INC_ROOT}/Async/SettlementStates.hpp
    ${INC_ROOT}/Async/WorkItemBase.hpp
    ${INC_ROOT}/Async/ExecutableWorkItemBase.hpp
    ${INC_ROOT}/Async/SchedulerBase.hpp
    ${INC_ROOT}/Async/ChainLinkerBase.hpp
    ${INC_ROOT}/Async/ContinuableWorkItemBase.hpp
    ${INC_ROOT}/Async/PromiseBase.hpp
    ${INC_ROOT}/Async/PromiseBaseImpl.hpp
    ${INC_ROOT}/Async/Promise.hpp
    ${INC_ROOT}/Async/ValueHolder.hpp
    ${INC_ROOT}/Async/ConcurrencyStates.hpp
    ${INC_ROOT}/Async/JobPriorities.hpp
    ${INC_ROOT}/Async/ThreadBase.hpp
    ${INC_ROOT}/Async/EngineBase.hpp
    ${INC_ROOT}/Async/PromiseLifeSaver.hpp
    ${INC_ROOT}/Async/SimpleChainLinker.hpp
    ${INC_ROOT}/Async/AsyncExecution.hpp
    ${INC_ROOT}/Async/Async.hpp
)

set( ASYNC_PRIVATE_HEADERS
    # logging
    ${CONFIG_ROOT}/AsyncConfig.hpp

    # ${INC_ROOT}/Async/WorkItemStateMachine.hpp
    ${INC_ROOT}/Async/WorkItem.hpp
    ${INC_ROOT}/Async/Scheduler.hpp
    # ${INC_ROOT}/Async/ContinuableWorkItem.hpp
    ${INC_ROOT}/Async/WorkerThread.hpp
    ${INC_ROOT}/Async/Engine.hpp
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

set( ASYNC_COMPILE_DEFINITIONS )

# check required libraries and header files and such.
# this will fail CMake if set to False due to required
# modules requiring their own dependencies
set( DEPENDENCY_CHECK FALSE )

include( GNUInstallDirs )

set( ${PROJECT_NAME}_IMPORTED_LIST
    # all libraries to import when building Async
    "Logging"
    "Utilities"
)
include(ProjectLinker)

if( NOT DEPENDENCY_CHECK )
    message( SEND_ERROR "Required header files or libraries not found" )
endif()

include_directories( ${INCLUDE_DIRS} )

# Logging library
set( ASYNC_LIB_NAME ${PROJECT_NAME} )

# Shared Library export header file supporte
include( GenerateExportHeader )

add_library( ${ASYNC_LIB_NAME} SHARED ${ASYNC_SRCS} ${ASYNC_PUBLIC_HEADERS} ${ASYNC_PRIVATE_HEADERS})
if( ${${PROJECT_NAME}_IMPORTED_LIBS_LENGTH} GREATER 0 )
    target_link_libraries( ${ASYNC_LIB_NAME} ${${PROJECT_NAME}_IMPORTED_LIBS} )
endif()

set_target_properties( ${ASYNC_LIB_NAME} PROPERTIES
    PUBLIC_HEADER       "${ASYNC_PUBLIC_HEADERS}"
    COMPILE_DEFINITIONS "${ASYNC_COMPILE_DEFINITIONS}"
    SOVERSION           "${PROJECT_VERSION_MAJOR}"
    VERSION             "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

# install targets
install( TARGETS ${ASYNC_LIB_NAME}
         RUNTIME        DESTINATION ${CMAKE_INSTALL_BINDIR}
         LIBRARY        DESTINATION ${CMAKE_INSTALL_LIBDIR}
         ARCHIVE        DESTINATION ${CMAKE_INSTALL_LIBDIR}
         PUBLIC_HEADER  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Async
)

if( MSVC )
    get_filename_component( PDB_DIR ${OUT_ROOT}/stuff/${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ABSOLUTE )
    install( FILES ${PDB_DIR}/${PROJECT_NAME}.pdb
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

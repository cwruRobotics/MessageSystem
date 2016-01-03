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
project( FuturesFrameworkUnitTests CXX )

# source code root directory
get_filename_component( SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE )

# Public header file root directory
get_filename_component( INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../include ABSOLUTE )

# ------------------------------------------------------------------------------
# GTest
#

if( RUN_UNIT_TESTS )
    enable_testing()
    
    list( APPEND CMAKE_FIND_ROOT_PATH ${INSTALL_ROOT} )
    # include(BringInPackages)  # NOT build in
    # BringInPackages()
    find_package( Catch REQUIRED )

    if( CATCH_FOUND )
        # ---------------------------------------------------------------------
        # Libararies
        # ---------------------------------------------------------------------

        # ---------------------------------------------------------------------
        # Targets Setup
        # ---------------------------------------------------------------------

        # source files
        set( FUTURESFRAMEWORK_UNITTEST_SOURCES
            # FuturesFramework sources
            ${SRC_ROOT}/../Result.cpp
            ${SRC_ROOT}/../WorkItemStates.cpp
            ${SRC_ROOT}/../WorkItemStateMachine.cpp
            ${SRC_ROOT}/../Scheduler.cpp
            ${SRC_ROOT}/../WorkItem.cpp
            ${SRC_ROOT}/../SettlementStates.cpp
            ${SRC_ROOT}/../ContinuableWorkItem.cpp
            ${SRC_ROOT}/../PromiseBase.cpp

            # FuturesFrameworkUnitTests sources
            ${SRC_ROOT}/catchConfig.cpp
            ${SRC_ROOT}/WorkItemStateMachineUnitTestInterface.cpp
            ${SRC_ROOT}/MockScheduler.cpp
            ${SRC_ROOT}/WorkItemStateMachine_unit.cpp
            ${SRC_ROOT}/WorkItem_unit.cpp
            ${SRC_ROOT}/WorkItemAndScheduler_unit.cpp
            ${SRC_ROOT}/ContinuableWorkItem_unit.cpp
            ${SRC_ROOT}/Promise_unit.cpp
        )

        set( FUTURESFRAMEWORK_UNITTEST_HEADERS
            # FuturesFramework headers
            ${INC_ROOT}/Result.hpp
            ${INC_ROOT}/WorkItemStates.hpp
            ${INC_ROOT}/WorkItemStateMachine.hpp
            ${INC_ROOT}/IWorkItem.hpp
            ${INC_ROOT}/IExecutableWorkItem.hpp
            ${INC_ROOT}/IScheduler.hpp
            ${INC_ROOT}/Scheduler.hpp
            ${INC_ROOT}/WorkItem.hpp
            ${INC_ROOT}/SettlementStates.hpp
            ${INC_ROOT}/ContinuableWorkItem.hpp
            ${INC_ROOT}/PromiseBase.hpp
            ${INC_ROOT}/IPromise.hpp
            ${INC_ROOT}/Promise.hpp

            # FuturesFrameworkUnitTests headers
            ${SRC_ROOT}/WorkItemStateMachineUnitTestInterface.hpp
            ${SRC_ROOT}/MockScheduler.hpp
        )

        # directories to search for header files
        set( INCLUDE_DIRS
            ${INC_ROOT}
            ${SRC_ROOT}/../..
            ${GTEST_INCLUDE_DIRS}
            ${CMAKE_CURRENT_SOURCE_DIR}/..
        )

        if( MSVC )
            include( GNUInstallDirs )
        elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
            include( GNUInstallDirs )
            set(TARGET_PTHREADS_LIB "pthread")
            set(TARGET_UUID_LIB "uuid")
        elseif( ${CMAKE_SYSTEM_NAME} MATCHES "OSX")
            include( GNUInstallDirs )
            set(TARGET_PTHREADS_LIB "pthread")
            set(TARGET_UUID_LIB "uuid")
        else()
            message(SEND_ERROR "OS [${CMAKE_SYSTEM_NAME}] not supported")
        endif()

        add_definitions(-DFUTURESFRAMEWORK_STATIC_DEFINE) #

        include_directories( ${INCLUDE_DIRS} )

        add_executable( ${PROJECT_NAME} ${FUTURESFRAMEWORK_UNITTEST_SOURCES} )

        # set_target_properties( ${PROJECT_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
        string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER )
        if( CMAKE_BUILD_TYPE_LOWER STREQUAL "debug" )
            message( "Linking executable for Debug" )
            target_link_libraries( ${PROJECT_NAME}
                ${TARGET_PTHREADS_LIB}
                ${TARGET_UUID_LIB}
            )
        else()
            message( "Linking executable for Release" )
            target_link_libraries( ${PROJECT_NAME}
                ${TARGET_PTHREADS_LIB}
                ${TARGET_UUID_LIB}
            )
        endif()

    else()
        message( WARNING "Catch library not found...skipping unit tests")
    endif()

endif()

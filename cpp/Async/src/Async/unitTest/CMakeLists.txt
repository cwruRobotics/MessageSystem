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
project( AsyncUnitTests CXX )

# source code root directory
get_filename_component( SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE )

# Public header file root directory
get_filename_component( INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../include ABSOLUTE )

get_filename_component( CONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../build/out/include ABSOLUTE )

get_filename_component( CATCH_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../../TestingFrameworks ABSOLUTE )

if( RUN_UNIT_TESTS )
    enable_testing()
    
    list( APPEND CMAKE_FIND_ROOT_PATH ${INSTALL_ROOT} )

    set( ${PROJECT_NAME}_IMPORTED_LIST
        # all libraries to import when building Async
        "Logging"
        "Utilities"
    )
    include(ProjectLinker)
    find_package( Catch REQUIRED )

    if( CATCH_FOUND )

        # source files
        set( ASYNC_UNITTEST_SOURCES
            # Logging sources
            ${CONFIG_ROOT}/AsyncConfig.cpp

            # Async sources
            ${SRC_ROOT}/../Result.cpp
            ${SRC_ROOT}/../WorkItemStates.cpp
            # ${SRC_ROOT}/../WorkItemStateMachine.cpp
            ${SRC_ROOT}/../Scheduler.cpp
            ${SRC_ROOT}/../WorkItem.cpp
            ${SRC_ROOT}/../SettlementStates.cpp
            # ${SRC_ROOT}/../ContinuableWorkItem.cpp
            ${SRC_ROOT}/../JobPriorities.cpp
            ${SRC_ROOT}/../PromiseBaseImpl.cpp
            ${SRC_ROOT}/../WorkerThread.cpp
            ${SRC_ROOT}/../Engine.cpp
            ${SRC_ROOT}/../Factories/EngineFactory.cpp

            # AsyncUnitTests sources
            ${SRC_ROOT}/catchConfig.cpp
            ${SRC_ROOT}/TestUtilities.cpp
            ${SRC_ROOT}/MockWorkItem.cpp
            # ${SRC_ROOT}/WorkItemStateMachineUnitTestInterface.cpp
            ${SRC_ROOT}/MockScheduler.cpp
            # ${SRC_ROOT}/WorkItemStateMachine_unit.cpp
            ${SRC_ROOT}/WorkItem_unit.cpp
            ${SRC_ROOT}/WorkItemAndScheduler_unit.cpp
            # ${SRC_ROOT}/ContinuableWorkItem_unit.cpp
            ${SRC_ROOT}/ValueHolder_unit.cpp
            ${SRC_ROOT}/Promise_unit.cpp
            ${SRC_ROOT}/WorkerThread_unit.cpp
            ${SRC_ROOT}/Engine_unit.cpp
            ${SRC_ROOT}/Async_unit.cpp
            ${SRC_ROOT}/AsyncExecution_unit.cpp
            ${SRC_ROOT}/SimpleContinuation_unit.cpp
        )

        set( ASYNC_UNITTEST_HEADERS
            # apparently we don't need to include Async headers.
            # I assume its because we are not exporting them.

            # AsyncUnitTests headers
            # ${SRC_ROOT}/WorkItemStateMachineUnitTestInterface.hpp
            ${SRC_ROOT}/TestUtilities.hpp
            ${SRC_ROOT}/MockWorkItem.hpp
            ${SRC_ROOT}/MockScheduler.hpp
        )

        # directories to search for header files
        set( INCLUDE_DIRS
            ${INC_ROOT}
            ${SRC_ROOT}/../..
            ${CMAKE_CURRENT_SOURCE_DIR}/..
            ${CONFIG_ROOT}
            ${CATCH_ROOT}
            ${CMAKE_PREFIX_PATH}/include
        )

        if( MSVC )
            include( GNUInstallDirs )
        elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
            include( GNUInstallDirs )
            set(TARGET_PTHREADS_LIB "pthread" )
        elseif( APPLE )
            include( GNUInstallDirs )
            set(TARGET_PTHREADS_LIB "pthread")
        else()
            message(SEND_ERROR "OS [${CMAKE_SYSTEM_NAME}] not supported")
        endif()

        add_definitions(-DASYNC_STATIC_DEFINE) #

        include_directories( ${INCLUDE_DIRS} )

        add_executable( ${PROJECT_NAME} ${ASYNC_UNITTEST_SOURCES} )
        if( ${${PROJECT_NAME}_IMPORTED_LIBS_LENGTH} GREATER 0 )
            target_link_libraries( ${PROJECT_NAME} ${${PROJECT_NAME}_IMPORTED_LIBS} )
        endif()

        string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER )
        if( CMAKE_BUILD_TYPE_LOWER STREQUAL "debug" )
            message( "Linking executable for Debug" )
            target_link_libraries( ${PROJECT_NAME}
                ${TARGET_PTHREADS_LIB}
            )
        else()
            message( "Linking executable for Release" )
            target_link_libraries( ${PROJECT_NAME}
                ${TARGET_PTHREADS_LIB}
            )
        endif()

    else()
        message( WARNING "Catch Framework not found...skipping unit tests")
    endif()

endif()

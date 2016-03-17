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
get_filename_component( CMAKE_LOCAL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../cmake ABSOLUTE )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_LOCAL_ROOT}/modules )
# ===============================================================================
# PROJECT PROPERTIES
# ===============================================================================

# Project name
project( UtilitiesUnitTests CXX )

# source code root directory
get_filename_component( SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE )

# Public header file root directory
get_filename_component( INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../include ABSOLUTE )

get_filename_component( CONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../build/out/include ABSOLUTE )

get_filename_component( CATCH_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../../TestingFrameworks ABSOLUTE )

# ------------------------------------------------------------------------------
# GTest
#

if( RUN_UNIT_TESTS )
    enable_testing()
    set( ENABLE_EXTERNAL_STRUCTURES_TEST "OFF" )
    
    list( APPEND CMAKE_FIND_ROOT_PATH ${INSTALL_ROOT} )

    # set( UTILITIES_LIBS )
    # if( ENABLE_LOGGING )
    #     find_package( Logging REQUIRED )
    #     if( LOGGING_FOUND )
    #         add_library( LOGGING SHARED IMPORTED )
    #         if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    #             set_property( TARGET LOGGING PROPERTY IMPORTED_LOCATION ${LOGGING_SHARED} )
    #             set_property( TARGET LOGGING PROPERTY IMPORTED_IMPLIB ${LOGGING_LIBRARY} )
    #         else()
    #             set_property( TARGET LOGGING PROPERTY IMPORTED_LOCATION ${LOGGING_LIBRARY} )
    #         endif()

    #         list( APPEND UTILITIES_LIBS LOGGING )

    #     else()
    #         message( WARNING "Logging not found...skipping usage" )
    #     endif()
    # endif()
    # list( LENGTH UTILITIES_LIBS UTILITIES_LIBS_LENGTH )

    set( ${PROJECT_NAME}_IMPORTED_LIST
        # all libraries to import when building Async
        "Logging"
    )
    include(ProjectLinker)
    find_package( Catch REQUIRED )

    if( CATCH_FOUND )
        # ---------------------------------------------------------------------
        # Libararies
        # ---------------------------------------------------------------------

        # ---------------------------------------------------------------------
        # Targets Setup
        # ---------------------------------------------------------------------

        # source files
        set( UTILITIES_UNITTEST_SOURCES
            # Logging sources
            ${CONFIG_ROOT}/UtilitiesLoggingConfig.cpp

            # Utilities sources
            ${SRC_ROOT}/../../Utilities/Semaphore.cpp

            # UtilitiesUnitTests sources
            ${SRC_ROOT}/catchConfig.cpp
            ${SRC_ROOT}/CommonMemory.cpp
            ${SRC_ROOT}/Waiter.cpp
            ${SRC_ROOT}/Signaler.cpp
            ${SRC_ROOT}/SimpleSemaphoreTest.cpp
            ${SRC_ROOT}/Semaphore_unit.cpp

        )

        if( ENABLE_EXTERNAL_STRUCTURES_TEST )
            LIST( APPEND UTILITIES_UNITTEST_SOURCES
                ${SRC_ROOT}/BoundTest.cc
                ${SRC_ROOT}/RelationTest.cc
                ${SRC_ROOT}/StructureTest.cc
                ${SRC_ROOT}/IteratorTest.cc
                ${SRC_ROOT}/InstantiationTest.cc
                ${SRC_ROOT}/DumpRestoreTest.cc
                ${SRC_ROOT}/LargeTest.cc
                ${SRC_ROOT}/BulkLoadTest.cc
            )
        endif()

        set( UTILITIES_UNITTEST_HEADERS
            # apparently we don't need to include Async headers.
            # I assume its because we are not exporting them.

            # UtilitiesUnitTests headers
            ${SRC_ROOT}/CommonMemory.hpp
            ${SRC_ROOT}/Signaler.hpp
            ${SRC_ROOT}/Waiter.hpp
            ${SRC_ROOT}/SimpleSemaphoreTest.hpp

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

        add_definitions(-DUTILITIES_STATIC_DEFINE) #

        include_directories( ${INCLUDE_DIRS} )

        add_executable( ${PROJECT_NAME} ${UTILITIES_UNITTEST_SOURCES} )
        if( ${${PROJECT_NAME}_IMPORTED_LIBS_LENGTH} GREATER 0 )
            target_link_libraries( ${PROJECT_NAME} ${${PROJECT_NAME}_IMPORTED_LIBS} )
        endif()

        # set_target_properties( ${PROJECT_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
        string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER )
        if( CMAKE_BUILD_TYPE_LOWER STREQUAL "debug" )
            message( "Linking executable for Debug" )
            target_link_libraries( ${PROJECT_NAME}
                ${TARGET_PTHREADS_LIB}
                # ${TARGET_UUID_LIB}
            )
        else()
            message( "Linking executable for Release" )
            target_link_libraries( ${PROJECT_NAME}
                ${TARGET_PTHREADS_LIB}
                # ${TARGET_UUID_LIB}
            )
        endif()

    else()
        message( WARNING "Catch library not found...skipping unit tests")
    endif()

endif()

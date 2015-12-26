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
project( TestAppUnitTests CXX )

# source code root directory
get_filename_component( SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE )

# Public header file root directory
get_filename_component( INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../include ABSOLUTE )
if( DEFINED GTEST_ROOT )
    list( APPEND CMAKE_MODULE_PATH ${GTEST_ROOT}/share/gtest/cmake/modules )
endif()

# ------------------------------------------------------------------------------
# GTest
#

if( RUN_UNIT_TESTS )
    enable_testing()
    
    list( APPEND CMAKE_FIND_ROOT_PATH ${INSTALL_ROOT} )
    # include(BringInPackages)  # NOT build in
    # BringInPackages()
    find_package( GTest REQUIRED )

    # Don't worry about this right now. This will import GTest and GMock
    # if they aren't found.
    if( NOT GTEST_FOUND )
        # try and import and build the project.
        include( ImportGTest )
        importGTest()

        find_package( GTest REQUIRED )
    endif()

    if( GTEST_FOUND )
        # ---------------------------------------------------------------------
        # Libararies
        # ---------------------------------------------------------------------

        # ---------------------------------------------------------------------
        # Targets Setup
        # ---------------------------------------------------------------------

        # source files
        set( TESTAPP_UNITTEST_SOURCES
            ${SRC_ROOT}/../testSource.cpp
            ${SRC_ROOT}/test.cpp
        )

        # directories to search for header files
        set( INCLUDE_DIRS
            ${INC_ROOT}
            ${SRC_ROOT}/../..
            ${GTEST_INCLUDE_DIRS}
            ${CMAKE_CURRENT_SOURCE_DIR}/..
        )

        if( MSVC )
            add_definitions( /D GTEST_HAS_TR1_TUPLE=0 )
            add_definitions( /D GTEST_USE_OWN_TR1_TUPLE=1 )
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

        add_definitions(-DTESTAPP_STATIC_DEFINE) # so testApp can be built as an executable
        include(${CMAKE_LOCAL_ROOT}/modules/internal_utils.cmake)

        fix_default_compiler_settings_()

        include_directories( ${INCLUDE_DIRS} )

        add_executable( ${PROJECT_NAME} ${TESTAPP_UNITTEST_SOURCES} )

        # set_target_properties( ${PROJECT_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
        string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER )
        if( CMAKE_BUILD_TYPE_LOWER STREQUAL "debug" )
            message( "GTEST_MAIN_LIBRARY_DEBUG: ${GTEST_MAIN_LIBRARY_DEBUG}" )
            message( "GTEST_LIBRARY_DEBUG: ${GTEST_LIBRARY_DEBUG}" )
            message( "Linking executable for Debug" )
            target_link_libraries( ${PROJECT_NAME}
                ${GTEST_LIBARAY_DEBUG}
                ${GTEST_MAIN_LIBRARY_DEBUG}
                ${TARGET_PTHREADS_LIB}
                ${TARGET_UUID_LIB}
            )
        else()
            message( "Linking executable for Release" )
            target_link_libraries( ${PROJECT_NAME}
                ${GTEST_LIBRARAY_RELEASE}
                ${GTEST_MAIN_LIBRARY_RELEASE}
                ${TARGET_PTHREADS_LIB}
                ${TARGET_UUID_LIB}
            )
        endif()

    else()
        message( WARNING "GTest library not found...skipping unit tests")
    endif()

endif()

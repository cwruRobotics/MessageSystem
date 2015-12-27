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
project( LoggingUnitTests CXX )

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
        set( LOGGING_UNITTEST_SOURCES
            ${SRC_ROOT}/../LoggingLevels.cpp
            ${SRC_ROOT}/../Logger.cpp
            ${SRC_ROOT}/../Factory.cpp
            ${SRC_ROOT}/LoggerTestHelper.cpp
            ${SRC_ROOT}/Logger_unit.cpp
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

        add_definitions(-DLOGGING_STATIC_DEFINE) # so the Logging library will be imported

        include_directories( ${INCLUDE_DIRS} )

        add_executable( ${PROJECT_NAME} ${LOGGING_UNITTEST_SOURCES} )

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

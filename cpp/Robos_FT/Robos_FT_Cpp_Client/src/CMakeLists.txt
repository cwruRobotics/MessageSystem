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
project( Robos_FT_Cpp_Client CXX )

# source code root directory
get_filename_component( SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE )

# Public header file root directory
get_filename_component( INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../include ABSOLUTE )

get_filename_component( CONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../../build/out/include ABSOLUTE )

# get_filename_component( TEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../TestingFrameworks ABSOLUTE )

# ------------------------------------------------------------------------------
# GTest
#

if( RUN_UNIT_TESTS )
    enable_testing()
    
    list( APPEND CMAKE_FIND_ROOT_PATH ${INSTALL_ROOT} )
    # include(BringInPackages)  # NOT build in
    # BringInPackages()
    find_package( gtest REQUIRED )

    if( GTEST_FOUND )
        message( "GTEST_LIBS: ${GTEST_LIBS}" )
        # ---------------------------------------------------------------------
        # Libararies
        # ---------------------------------------------------------------------

        # ---------------------------------------------------------------------
        # Targets Setup
        # ---------------------------------------------------------------------

        # source files
        set( ${PROJECT_NAME}_SOURCES
            # non test code

            # test code
            ${SRC_ROOT}/Robos_test.cpp
            ${SRC_ROOT}/main.cpp
        )

        set( ${PROJECT_NAME}_HEADERS
            
        )

        # directories to search for header files
        set( INCLUDE_DIRS
            ${INC_ROOT}
            ${SRC_ROOT}/../..
            ${CONFIG_ROOT}
            ${CMAKE_CURRENT_SOURCE_DIR}/..
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

        set( ${PROJECT_NAME}_IMPORTED_LIST
            # all libraries to import when building Utilities_FT
            "Utilities"
            "Robos"
            "gtest"
        )
        include( ProjectLinker )

        include_directories( ${INCLUDE_DIRS} )

        add_executable( ${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS} )

        # set_target_properties( ${PROJECT_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
        string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER )
        if( CMAKE_BUILD_TYPE_LOWER STREQUAL "debug" )
            message( "Linking executable for Debug" )
            target_link_libraries( ${PROJECT_NAME}
                ${TARGET_PTHREADS_LIB}
                ${${PROJECT_NAME}_IMPORTED_LIBS}
                # ${GTEST_LIBS}
                # ${TARGET_UUID_LIB}
            )
        else()
            message( "Linking executable for Release" )
            target_link_libraries( ${PROJECT_NAME}
                ${TARGET_PTHREADS_LIB}
                ${${PROJECT_NAME}_IMPORTED_LIBS}
                # ${GTEST_LIBS}
                # ${TARGET_UUID_LIB}
            )
        endif()

    else()
        message( WARNING "gtest library not found...skipping tests")
    endif()

endif()

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
project( RobosUnitTests CXX )

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
        # all libraries to import when building Robos
        "Logging"
        "Utilities"
        "Async"
    )
    include(ProjectLinker)
    find_package( Catch REQUIRED )

    if( CATCH_FOUND )

        # source files
        set( ROBOS_UNITTEST_SOURCES
            # Logging sources
            ${CONFIG_ROOT}/RobosConfig.cpp

            # Robos sources
            ${SRC_ROOT}/../MessageBase.cpp
            ${SRC_ROOT}/../NodeHelper.cpp
            ${SRC_ROOT}/../NodeBase.cpp
            ${SRC_ROOT}/../InitNodeBase.cpp
            ${SRC_ROOT}/../NodeDatabase.cpp
            ${SRC_ROOT}/../MasterNode.cpp
            ${SRC_ROOT}/../Robos.cpp

            # RobosUnitTests sources
            ${SRC_ROOT}/TestMessageA.cpp
            ${SRC_ROOT}/TestMessageB.cpp
            ${SRC_ROOT}/TestMessageC.cpp
            ${SRC_ROOT}/TestCountMessage.cpp

            ${SRC_ROOT}/catchConfig.cpp
            ${SRC_ROOT}/TestNode.cpp
            ${SRC_ROOT}/TestInitNode.cpp
            ${SRC_ROOT}/TestNodeNameTemplate.cpp
            ${SRC_ROOT}/TestNodeSubscriberTemplate.cpp
            ${SRC_ROOT}/CountTestNode.cpp
            ${SRC_ROOT}/TestCountInitNode.cpp
            ${SRC_ROOT}/ShutdownTestNode.cpp
            ${SRC_ROOT}/MessageBase_unit.cpp
            ${SRC_ROOT}/NodeDatabase_unit.cpp
            ${SRC_ROOT}/MasterNode_unit.cpp
            ${SRC_ROOT}/Robos_unit.cpp
        )

        set( ROBOS_UNITTEST_HEADERS

            # RobosUnitTests headers
            ${SRC_ROOT}/TestMessageA.hpp
            ${SRC_ROOT}/TestMessageB.hpp
            ${SRC_ROOT}/TestMessageC.hpp
            ${SRC_ROOT}/TestCountMessage.hpp
            ${SRC_ROOT}/TestNode.hpp
            ${SRC_ROOT}/TestInitNode.hpp
            ${SRC_ROOT}/TestNodeNameTemplate.hpp
            ${SRC_ROOT}/TestNodeSubscriberTemplate.hpp
            ${SRC_ROOT}/CountTestNode.hpp
            ${SRC_ROOT}/TestCountInitNode.hpp
            ${SRC_ROOT}/ShutdownTestNode.hpp
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
            set(TARGET_PTHREADS_LIB "pthread")
        elseif( APPLE )
            include( GNUInstallDirs )
            set(TARGET_PTHREADS_LIB "pthread")
        else()
            message(SEND_ERROR "OS [${CMAKE_SYSTEM_NAME}] not supported")
        endif()

        add_definitions(-DROBOS_STATIC_DEFINE) #

        include_directories( ${INCLUDE_DIRS} )

        add_executable( ${PROJECT_NAME} ${ROBOS_UNITTEST_SOURCES} )
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

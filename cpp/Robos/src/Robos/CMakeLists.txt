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
project( Robos CXX )

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
set( ROBOS_SRCS
    # logging
    # ${CONFIG_ROOT}/RobosLoggingConfig.cpp
    ${CONFIG_ROOT}/RobosConfig.cpp

    ${SRC_ROOT}/Robos/MessageBase.cpp
    ${SRC_ROOT}/Robos/NodeHelper.cpp
    ${SRC_ROOT}/Robos/NodeBase.cpp
    ${SRC_ROOT}/Robos/InitNodeBase.cpp
    ${SRC_ROOT}/Robos/NodeDatabase.cpp
    ${SRC_ROOT}/Robos/MasterNode.cpp
    ${SRC_ROOT}/Robos/Robos.cpp

    # factories
    # ${SRC_ROOT}/Robos/Factories/EngineFactory.cpp
)

# Public header files (installed together with libraries)
set( ROBOS_PUBLIC_HEADERS
    ${INC_ROOT}/Robos/LibraryExport.hpp
    ${INC_ROOT}/Robos/MessageBase.hpp
    ${INC_ROOT}/Robos/NodeHelper.hpp
    ${INC_ROOT}/Robos/NodeBase.hpp
    ${INC_ROOT}/Robos/InitNodeBase.hpp
    ${INC_ROOT}/Robos/Robos.hpp
)

set( ROBOS_PRIVATE_HEADERS
    # logging
    ${CONFIG_ROOT}/RobosConfig.hpp

    ${INC_ROOT}/Robos/NodeDatabase.hpp
    ${INC_ROOT}/Robos/MasterNode.hpp
)

if( PYTHON_ENABLED )
    list( APPEND ROBOS_SRCS
        ${SRC_ROOT}/Robos/PyNodeBase.cpp
    )
    list( APPEND ROBOS_PUBLIC_HEADERS
        ${INC_ROOT}/Robos/PyNodeBase.hpp
    )
endif()

# setup visual studio source groups
source_group( PublicHeaders FILES ${ROBOS_PUBLIC_HEADERS} )

# Directories to search for header files
set( INCLUDE_DIRS
    ${INC_ROOT}
    ${SRC_ROOT}
    ${CONFIG_ROOT}
)

set( ROBOS_COMPILE_DEFINITIONS )

# check required libraries and header files and such.
# this will fail CMake if set to False due to required
# modules requiring their own dependencies
set( DEPENDENCY_CHECK FALSE )

include( GNUInstallDirs )

set( ${PROJECT_NAME}_IMPORTED_LIST
    # all libraries to import when building Robos
    "Logging"
    "Utilities"
    "Async"
)
if( PYTHON_ENABLED )
    set( ${PROJECT_NAME}_STATIC_LIST
        "Python"
    )
endif()
include(ProjectLinker)

list( APPEND INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDES} )

if( NOT DEPENDENCY_CHECK )
    message( SEND_ERROR "Required header files or libraries not found" )
endif()

include_directories( ${INCLUDE_DIRS} )

# Logging library
set( ROBOS_LIB_NAME ${PROJECT_NAME} )

# Shared Library export header file supporte
include( GenerateExportHeader )

add_library( ${ROBOS_LIB_NAME} SHARED ${ROBOS_SRCS} ${ROBOS_PUBLIC_HEADERS} ${ROBOS_PRIVATE_HEADERS})
if( ${${PROJECT_NAME}_IMPORTED_LIBS_LENGTH} GREATER 0 )
    target_link_libraries( ${ROBOS_LIB_NAME} ${${PROJECT_NAME}_IMPORTED_LIBS} )
endif()
if( ${${PROJECT_NAME}_STATIC_LIBS_LENGTH} GREATER 0 )
    target_link_libraries( ${ROBOS_LIB_NAME} ${${PROJECT_NAME}_STATIC_LIBS} )
endif()

set_target_properties( ${ROBOS_LIB_NAME} PROPERTIES
    PUBLIC_HEADER       "${ROBOS_PUBLIC_HEADERS}"
    COMPILE_DEFINITIONS "${ROBOS_COMPILE_DEFINITIONS}"
    SOVERSION           "${PROJECT_VERSION_MAJOR}"
    VERSION             "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

# install targets
install( TARGETS ${ROBOS_LIB_NAME}
         RUNTIME        DESTINATION ${CMAKE_INSTALL_BINDIR}
         LIBRARY        DESTINATION ${CMAKE_INSTALL_LIBDIR}
         ARCHIVE        DESTINATION ${CMAKE_INSTALL_LIBDIR}
         PUBLIC_HEADER  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Robos
)

if( MSVC )
    get_filename_component( PDB_DIR ${OUT_ROOT}/stuff/${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ABSOLUTE )
    install( FILES ${PDB_DIR}/${PROJECT_NAME}.pdb
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

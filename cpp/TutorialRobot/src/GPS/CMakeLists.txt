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
project( TutorialRobot_GPS CXX )

# source code root directory
get_filename_component( SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE )

# Public header file root directory
get_filename_component( INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../include/GPS ABSOLUTE )

get_filename_component( CONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../../build/out/include ABSOLUTE )

# this is where we could get the Version numbering from Version.h and
# save them as CMake variables. for now, lets just declare them
set( PROJECT_VERSION_MAJOR "1" )
set( PROJECT_VERSION_MINOR "0" )
set( PROJECT_VERSION_PATCH "0" )

# Source files
set( TUTORIALROBOT_GPS_SRCS
    # logging
    # ${CONFIG_ROOT}/RobosLoggingConfig.cpp
    ${CONFIG_ROOT}/TutorialRobotConfig.cpp

    ${SRC_ROOT}/Messages/GPSMessage.cpp
    ${SRC_ROOT}/Project_Code/GenerateGPSData.cpp
    ${SRC_ROOT}/Nodes/GPSSensorNode.cpp

    # factories
    # ${SRC_ROOT}/Robos/Factories/EngineFactory.cpp
)

# Public header files (installed together with libraries)
set( TUTORIALROBOT_GPS_PUBLIC_HEADERS
    ${INC_ROOT}/LibraryExport.hpp
    ${INC_ROOT}/Messages/GPSMessage.hpp
    ${INC_ROOT}/Nodes/GPSSensorNode.hpp
)

set( TUTORIALROBOT_GPS_PRIVATE_HEADERS
    # logging
    ${CONFIG_ROOT}/TutorialRobotConfig.hpp

    ${INC_ROOT}/Project_Code/GenerateGPSData.hpp
)

# setup visual studio source groups
source_group( PublicHeaders FILES ${ROBOS_PUBLIC_HEADERS} )

# Directories to search for header files
set( INCLUDE_DIRS
    ${INC_ROOT}
    ${SRC_ROOT}
    ${CONFIG_ROOT}
    # ${CMAKE_PREFIX_PATH}/include
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
include(ProjectLinker)

list( APPEND INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDES} )

if( NOT DEPENDENCY_CHECK )
    message( SEND_ERROR "Required header files or libraries not found" )
endif()

include_directories( ${INCLUDE_DIRS} )

set( TUTORIALROBOT_GPS_LIB_NAME ${PROJECT_NAME} )

# Shared Library export header file supporte
include( GenerateExportHeader )

add_library( ${TUTORIALROBOT_GPS_LIB_NAME} SHARED ${TUTORIALROBOT_GPS_SRCS} ${TUTORIALROBOT_GPS_PUBLIC_HEADERS} ${TUTORIALROBOT_GPS_PRIVATE_HEADERS})
if( ${${PROJECT_NAME}_IMPORTED_LIBS_LENGTH} GREATER 0 )
    target_link_libraries( ${TUTORIALROBOT_GPS_LIB_NAME} ${${PROJECT_NAME}_IMPORTED_LIBS} )
endif()
if( ${${PROJECT_NAME}_STATIC_LIBS_LENGTH} GREATER 0 )
    target_link_libraries( ${TUTORIALROBOT_GPS_LIB_NAME} ${${PROJECT_NAME}_STATIC_LIBS} )
endif()

set_target_properties( ${TUTORIALROBOT_GPS_LIB_NAME} PROPERTIES
    PUBLIC_HEADER       "${TUTORIALROBOT_GPS_PUBLIC_HEADERS}"
    COMPILE_DEFINITIONS "${TUTORIALROBOT_GPS_COMPILE_DEFINITIONS}"
    SOVERSION           "${PROJECT_VERSION_MAJOR}"
    VERSION             "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

# install targets
install( TARGETS ${TUTORIALROBOT_GPS_LIB_NAME}
         RUNTIME        DESTINATION ${OUT_ROOT}/bin #${CMAKE_INSTALL_BINDIR}
         LIBRARY        DESTINATION ${OUT_ROOT}/lib #${CMAKE_INSTALL_LIBDIR}
         ARCHIVE        DESTINATION ${OUT_ROOT}/lib #${CMAKE_INSTALL_LIBDIR}
         PUBLIC_HEADER  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TutorialRobot
)

if( MSVC )
    get_filename_component( PDB_DIR ${OUT_ROOT}/stuff/${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ABSOLUTE )
    install( FILES ${PDB_DIR}/${PROJECT_NAME}.pdb
        DESTINATION ${OUT_ROOT}/bin #${CMAKE_INSTALL_BINDIR}
    )
endif()

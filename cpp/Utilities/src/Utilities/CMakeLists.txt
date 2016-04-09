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
project( Utilities CXX )

# source code root directory
get_filename_component( SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/.. ABSOLUTE )

# Public header file root directory
get_filename_component( INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../include ABSOLUTE )

get_filename_component( CONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../../build/out/include ABSOLUTE )

# ===============================================================================
# PROJECT EXTERNAL OPTIONS
# ===============================================================================
# this is where we could get the Version numbering from Version.h and
# save them as CMake variables. for now, lets just declare them
set( PROJECT_VERSION_MAJOR "1" )
set( PROJECT_VERSION_MINOR "0" )
set( PROJECT_VERSION_PATCH "0" )

# -------------------------------------------------------------------------------
# Logging library

# Source files
set( UTILITIES_SRCS
    # logging
    ${CONFIG_ROOT}/UtilitiesConfig.cpp

    # utilities
    ${SRC_ROOT}/Utilities/Semaphore.cpp
    ${SRC_ROOT}/Utilities/OSUtils.cpp
    ${SRC_ROOT}/Utilities/PythonBridge.cpp

    # factories

)

# Public header files (installed together with libraries)
set( UTILITIES_PUBLIC_HEADERS
    # project code
    ${INC_ROOT}/Utilities/LibraryExport.hpp
    ${INC_ROOT}/Utilities/Semaphore.hpp
    ${INC_ROOT}/Utilities/OSUtils.hpp
    ${INC_ROOT}/Utilities/PythonInclude.hpp
    ${INC_ROOT}/Utilities/PythonBridge.hpp

    # third party library data structures
    ${INC_ROOT}/Utilities/btree.h
    ${INC_ROOT}/Utilities/btree_map.h
    ${INC_ROOT}/Utilities/btree_multimap.h
    ${INC_ROOT}/Utilities/btree_multiset.h
    ${INC_ROOT}/Utilities/btree_set.h

    # third party library xml parser
    ${INC_ROOT}/Utilities/manual.html
    ${INC_ROOT}/Utilities/rapidxml.hpp
    ${INC_ROOT}/Utilities/rapidxml_iterators.hpp
    ${INC_ROOT}/Utilities/rapidxml_print.hpp
    ${INC_ROOT}/Utilities/rapidxml_utils.hpp
)

set( UTILITIES_PRIVATE_HEADERS
    # logging
    ${CONFIG_ROOT}/UtilitiesConfig.hpp

    # factories
)

# setup visual studio source groups
source_group( PublicHeaders FILES ${UTILITIES_PUBLIC_HEADERS} )

# Directories to search for header files
set( INCLUDE_DIRS
    ${INC_ROOT}
    ${SRC_ROOT}
    ${CONFIG_ROOT}
    ${CMAKE_PREFIX_PATH}/include
    ${PYTHON_PATH}/include
)

# load external modules
# (none yet)...Logging does not depend on anything else

set( ENABLE_EXTERNAL_STRUCTURES_TEST "OFF" )

# set target specific compiler definitions
# this should ONLY be definitions needed to build the source code.
# examples: macros ON/OFF, etc.
set( UTILITIES_COMPILE_DEFINITIONS )

# check required libraries and header files and such.
# this will fail CMake if set to False due to required
# modules requiring their own dependencies
set( DEPENDENCY_CHECK FALSE )

# OS specific files and definitions
# if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )

#     include( ${SRC_ROOT}/Logging/Windows/Windows.cmake )
#     include( GNUInstallDirs ) # TODO make a Windows module if necessary
    
# elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    
#     include( ${SRC_ROOT}/Logging/Linux/Linux.cmake )
#     include( GNUInstallDirs ) # TODO make a Windows module if necessary

# elseif( ${CMAKE_SYSTEM_NAME} MATCHES "OSX" )

#     include( ${SRC_ROOT}/Logging/OSX/OSX.cmake )
#     include( GNUInstallDirs ) # TODO make a Windows module if necessary

# else()
#     message( SEND_ERROR "${CMAKE_SYSTEM_NAME} system build not supported" )
# endif()

include( GNUInstallDirs )

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
set( ${PROJECT_NAME}_STATIC_LIST
    "Python"
)

include(ProjectLinker)

if( NOT DEPENDENCY_CHECK )
    message( SEND_ERROR "Required header files or libraries not found" )
endif()


# -------------------------------------------------------------------------------
# TARGET SETUP
# -------------------------------------------------------------------------------

include_directories( ${INCLUDE_DIRS} )

# -------------------------------------------------------------------------------
# Libraries
#

# Logging library
set( UTILITIES_LIB_NAME ${PROJECT_NAME} )

# Shared Library export header file supporte
include( GenerateExportHeader )

add_library( ${UTILITIES_LIB_NAME} SHARED ${UTILITIES_SRCS} ${UTILITIES_PUBLIC_HEADERS} ${UTILITIES_PRIVATE_HEADERS})
if( ${${PROJECT_NAME}_IMPORTED_LIBS_LENGTH} GREATER 0 )
    target_link_libraries( ${UTILITIES_LIB_NAME} ${${PROJECT_NAME}_IMPORTED_LIBS} )
endif()
if( ${${PROJECT_NAME}_STATIC_LIBS_LENGTH} GREATER 0 )
    target_link_libraries( ${UTILITIES_LIB_NAME} ${${PROJECT_NAME}_STATIC_LIBS} )
endif()

set_target_properties( ${UTILITIES_LIB_NAME} PROPERTIES
    PUBLIC_HEADER       "${UTILITIES_PUBLIC_HEADERS}"
    COMPILE_DEFINITIONS "${UTILITIES_COMPILE_DEFINITIONS}"
    SOVERSION           "${PROJECT_VERSION_MAJOR}"
    VERSION             "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

# -------------------------------------------------------------------------------
# INSTALLATION
# -------------------------------------------------------------------------------

# install targets
install( TARGETS ${UTILITIES_LIB_NAME}
         RUNTIME        DESTINATION ${CMAKE_INSTALL_BINDIR}
         LIBRARY        DESTINATION ${CMAKE_INSTALL_LIBDIR}
         ARCHIVE        DESTINATION ${CMAKE_INSTALL_LIBDIR}
         PUBLIC_HEADER  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Utilities
)

if( MSVC )
    get_filename_component( PDB_DIR ${OUT_ROOT}/stuff/${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ABSOLUTE )
    install( FILES ${PDB_DIR}/${PROJECT_NAME}.pdb
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

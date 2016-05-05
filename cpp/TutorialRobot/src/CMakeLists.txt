# this is the required version of CMake necessary
# to run. CMake will fail if you have anything
# less than 3.3
cmake_minimum_required( VERSION 3.3 )

# when CMake generates Makefiles, this setting
# will allow them to print to the console.
set( CMAKE_VERBOSE_MAKEFILE "ON" )

# this points to the root of the cmake directory
# which contains all the toolchains and modules
get_filename_component( CMAKE_LOCAL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake ABSOLUTE )

# add our custom module path to the CMAKE_MODULE_PATH
# this allows us to say include( <module_name> )
# if <module_name> is located in ${CMAKE_LOCAL_ROOT}/modules
list( APPEND CMAKE_MODULE_PATH ${CMAKE_LOCAL_ROOT}/modules )

# this is the project name and type
project( Robos CXX )

get_filename_component( SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE )

option( RUN_UNIT_TESTS "Unit test support (default OFF)" OFF )
option( RUN_CPP_CHECK  "Static analysis support (default OFF)" OFF )

# documentation variables...ignore for now
set(DOC_PUBLIC_INPUT_LIST )
set(DOC_PRIVATE_INPUT_LIST )

add_subdirectory( ${SRC_ROOT}/GPS)
#                  ${OUT_ROOT}/TutorialRobot )

# give other subprojects a chance to search for libraries and header
# files created before the "make install"
list( APPEND CMAKE_INCLUDE_PATH "${OUT_ROOT}/TutorialRobot" )
list( APPEND CMAKE_LIBRARY_PATH "${OUT_ROOT}/TutorialRobot" )

# this variable is also set through the python build scripts.
if( RUN_UNIT_TESTS )

    add_subdirectory( ${SRC_ROOT}/GPS/unitTest )
#                      ${OUT_ROOT}/TutorialRobot )

endif()
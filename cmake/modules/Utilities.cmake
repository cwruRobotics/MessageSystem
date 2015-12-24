

# these are more or less useless. I am trying to make CMake
# functions that we can use. However, they aren't working yet.

function( findPath _name _root _toFind _configuration )
    find_path( ${_name}_INCLUDE_DIR ${_toFind}
        HINTS
            $ENV{${_root}}/include
            ${_root}/include
            $ENV{${_root}}/${_configuration}/include
            ${_root}/${_configuration}/include
    )
    SET( ${_name}_INCLUDE_DIR ${${_name}_INCLUDE_DIR} PARENT_SCOPE )
endfunction()

function( findLibrary _name _root _toFind _configuration )
    find_library( ${_name}_LIBRARY_${_configuration} ${_toFind}
        HINTS
            $ENV{${_root}}/${_configuration}
            ${_root}/${_configuration}
    )
    SET( ${_name}_LIBRARY_${_configuration} ${${_name}_LIBRARY_${_configuration}} PARENT_SCOPE )
endfunction()


function( find packageName FailOption toFindList )
    foreach( toFind ${toFindList} )
        
    endforeach()
endfunction()

function( findInclude )

function( correctBuildType )
    string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER )
    if( CMAKE_BUILD_TYPE_LOWER STREQUAL "debug" )
        set( CMAKE_BUILD_TYPE_ "Debug" )
    else()
        set( CMAKE_BUILD_TYPE_ "Release" )
    endif()
endfunction()

function( iterate packageName failOption valuesToSet filesToFind )
    LIST( LENGTH valuesToSet valueRange )
    MATH( EXPR len "${valueRange} - 1" )

    foreach( index RANGE ${len} )
        LIST( GET valuesToSet ${index} valueToSet )
        LIST( GET filesToFind ${index} fileToFind )
        message( "value for ${packageName} with config ${CMAKE_BUILD_TYPE_}: [${valueToSet}, ${fileToFind}]" )

        find_path( valueToSet ${fileToFind}
            HINTS
                $ENV{GTEST_ROOT}/include
                ${GTEST_ROOT}/include
                $ENV{GTEST_ROOT}/Debug
                ${GTEST_ROOT}/Debug
                $ENV{GTEST_ROOT}/Release
                ${GTEST_ROOT}/Release

        SET( valueToSet ${valueToSet} PARENT_SCOPE )
        message( "checking value: ${valueToSet}" )
        if( ${valueToSet} )
            message( "value ${valueToSet} is set" )
        else()
            message( "value ${valueToSet} is not set" )
        endif()
        )
    endforeach()
    set( GTEST_FOUND TRUE PARENT_SCOPE )
endfunction()

# find(GTest REQUIRED)
SET( FILESLIST
    "gtest/gtest.h" "gtest_main.lib" 
    "gtest.lib" "gtest_main.lib" "gtest.lib"
)
correctBuildType()
include( ${CMAKE_CURRENT_LIST_DIR}/Utilities.cmake )
find( GTest REQUIRED "${FILESLIST}" )
# message( "GTEST_INCLUDE_DIR: ${GTEST_INCLUDE_DIR}" )
# message( "GTEST_MAIN_LIBRARY: ${GTEST_MAIN_LIBRARY}" )


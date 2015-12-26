
# refactor this to set variables dependent on Debug or Release
# build type (variable is ${CMAKE_BUILD_TYPE})

find_path( GTEST_INCLUDE_DIRS gtest/gtest.h
    HINTS
        $ENV{GTEST_ROOT}/include/gtest
        ${GTEST_ROOT}/include/gtest
)

# if( NOT GTEST_INCLUDE_DIRS )
#     message( SEND_ERROR "GTEST INCLUDE DIR NOT FOUND" )
# endif()
if( GTEST_INCLUDE_DIRS )
    set( INCLUDE_FOUND TRUE )
else()
    set( INCLUDE_FOUND FALSE )
endif()

find_library( GTEST_LIBRARY gtest.lib
    HINTS
        $ENV{GTEST_ROOT}/lib
        ${GTEST_ROOT}/lib
)

find_library( GTEST_MAIN_LIBRARY gtest_main.lib
    HINTS
        $ENV{GTEST_ROOT}/lib
        ${GTEST_ROOT}/lib
)

message( "GTEST_INCLUDE_DIRS: ${GTEST_INCLUDE_DIRS}" )
message( "GTEST_LIBRARY: ${GTEST_LIBRARY}" )
message( "GTEST_MAIN_LIBRARY: ${GTEST_MAIN_LIBRARY}" )

string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER )
if( CMAKE_BUILD_TYPE_LOWER STREQUAL "debug" )

    set( GTEST_LIBRARY_DEBUG ${GTEST_LIBRARY} )
    set( GTEST_MAIN_LIBRARY_DEBUG ${GTEST_MAIN_LIBRARY} )

    # if( NOT GTEST_LIBRARY_DEBUG )
    #     message( SEND_ERROR "GTEST LIBRARY NOT FOUND" )
    # elseif( NOT GTEST_MAIN_LIBRARY_DEBUG )
    #     message( SEND_ERROR "GTEST MAIN LIBRARY NOT FOUND" )
    # endif()
    if( GTEST_LIBRARY_DEBUG AND GTEST_MAIN_LIBRARY_DEBUG )
        set( LIBRARIES_FOUND TRUE )
    else()
        set( LIBRARIES_FOUND FALSE )
    endif()

else()

    set( GTEST_LIBRARY_RELEASE ${GTEST_LIBRARY} )
    set( GTEST_MAIN_LIBRARY_RELEASE ${GTEST_MAIN_LIBRARY} )

    # if( NOT GTEST_LIBRARY_RELEASE )
    #     message( SEND_ERROR "GTEST LIBRARY NOT FOUND" )
    # elseif( NOT GTEST_MAIN_LIBRARY_RELEASE )
    #     message( SEND_ERROR "GTEST MAIN LIBRARY NOT FOUND" )
    # endif()
    if( GTEST_LIBRARY_RELEASE AND GTEST_MAIN_LIBRARY_RELEASE )
        set( LIBRARIES_FOUND TRUE )
    else()
        set( LIBRARIES_FOUND FALSE )
    endif()

endif()
if( INCLUDE_FOUND AND LIBRARIES_FOUND )
    set( GTEST_FOUND TRUE )
else()
    set( GTEST_FOUND FALSE )
endif()

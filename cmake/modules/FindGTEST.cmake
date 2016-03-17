
find_path( GTEST_INCLUDE_DIRS gtest/gtest.h
    HINTS
        ${INC_ROOT}
        ${CMAKE_INSTALL_PREFIX}/include
)

find_library( GTEST_LIB gtest
    HINTS
        ${CMAKE_INSTALL_PREFIX}/lib
)

find_library( GTEST_MAIN_LIB gtest_main
    HINTS
        ${CMAKE_INSTALL_PREFIX}/lib
)

if( GTEST_INCLUDE_DIRS AND GTEST_LIB AND GTEST_MAIN_LIB )
    set( GTEST_FOUND TRUE )
    set( GTEST_LIBS
        ${GTEST_LIB}
        # ${GTEST_MAIN_LIB}
    )
else()
    set( GTEST_FOUND FALSE )
endif()


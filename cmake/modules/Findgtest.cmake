
find_path( GTEST_INCLUDE_DIRS gtest/gtest.h
    HINTS
        ${INC_ROOT}
        ${CMAKE_INSTALL_PREFIX}/include
)

# find_library( GTEST_SHARED_LIB gtest
#     HINTS
#         ${CMAKE_INSTALL_PREFIX}/lib
# )

# find_library( GTEST_MAIN_LIB gtest_main
#     HINTS
#         ${CMAKE_INSTALL_PREFIX}/lib
# )

# if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
#     find_path( GTEST_LIB gtest.dll
#     
#     )
# endif()

# find libraries depending on OS.
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    # handle dll AND lib?
    find_path( GTEST_SHARED_LIB gtest.dll
        HINTS
            ${CMAKE_INSTALL_PREFIX}/bin
    )
    find_library( GTEST_LIB gtest
        HINTS
            ${CMAKE_INSTALL_PREFIX}/lib
    )
else()
    find_library( GTEST_SHARED_LIB gtest
        HINTS
            ${CMAKE_INSTALL_PREFIX}/lib
    )
endif()

if( GTEST_INCLUDE_DIRS AND GTEST_SHARED_LIB )
    set( GTEST_FOUND TRUE )
else()
    set( GTEST_FOUND FALSE )
endif()


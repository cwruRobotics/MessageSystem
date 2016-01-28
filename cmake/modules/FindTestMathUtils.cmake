
find_path( TESTMATHUTILS_INCLUDE_DIRS TestMathUtils/MySquare.hpp
    HINTS
        ${CMAKE_PREFIX_PATH}/include
)

find_library( TESTMATHUTILS_LIBRARY TestMathUtils
    HINTS
        ${CMAKE_PREFIX_PATH}/lib
)

# find libraries depending on OS.
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    # handle dll AND lib?
    find_path( TESTMATHUTILS_SHARED TestMathUtils.dll
        HINTS
            ${CMAKE_PREFIX_PATH}/bin
    )
endif()

if( NOT TESTMATHUTILS_INCLUDE_DIRS OR NOT TESTMATHUTILS_LIBRARY )
    set( TESTMATHUTILS_FOUND FALSE )
else()
    set( TESTMATHUTILS_FOUND TRUE )
endif()

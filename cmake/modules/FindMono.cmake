# need jit to run
find_path( MONO_INCLUDE_DIRS jit.h
    HINTS
        ${MONO_BASE_PATH}/include/mono-2.0/mono/jit
)

# find libraries depending on OS.
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    # handle dll AND lib?
    find_path( MONO_SHARED_LIB mono-2.0.dll
        HINTS
            ${MONO_BASE_PATH}/bin
    )
    find_library( MONO_LIB mono-2.0
        HINTS
            ${MONO_BASE_PATH}/lib
    )
else()
    find_library( MONO_SHARED_LIB mono-2.0
        HINTS
            ${MONO_BASE_PATH}/lib
    )
endif()

if( NOT MONO_INCLUDE_DIRS OR NOT MONO_SHARED_LIB )
    set( MONO_FOUND FALSE )
else()
    set( MONO_FOUND TRUE )
endif()

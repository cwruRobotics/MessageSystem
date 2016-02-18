find_path( ASYNC_INCLUDE_DIRS Async/AsyncExecution.hpp
    HINTS
        ${CMAKE_PREFIX_PATH}/include
)

# find libraries depending on OS.
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    # handle dll AND lib?
    find_path( ASYNC_SHARED_LIB Async.dll
        HINTS
            ${CMAKE_PREFIX_PATH}/bin
    )
    find_library( ASYNC_LIB Async
        HINTS
            ${CMAKE_PREFIX_PATH}/lib
    )
else()
    find_library( ASYNC_SHARED_LIB Async
        HINTS
            ${CMAKE_PREFIX_PATH}/lib
    )
endif()

if( NOT ASYNC_INCLUDE_DIRS OR NOT ASYNC_SHARED_LIB )
    set( ASYNC_FOUND FALSE )
else()
    set( ASYNC_FOUND TRUE )
endif()


find_path( LOGGING_INCLUDE_DIRS Logging/ILogger.hpp
    HINTS
        ${CMAKE_PREFIX_PATH}/include
)
find_library( LOGGING_LIBRARY Logging
    HINTS
        ${CMAKE_PREFIX_PATH}/lib
)

# find libraries depending on OS.
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    # handle dll AND lib?
    find_path( LOGGING_SHARED Logging.dll
        HINTS
            ${CMAKE_PREFIX_PATH}/bin
    )
endif()

if( NOT LOGGING_INCLUDE_DIRS OR NOT LOGGING_LIBRARY )
    set( LOGGING_FOUND FALSE )
else()
    set( LOGGING_FOUND TRUE )
endif()

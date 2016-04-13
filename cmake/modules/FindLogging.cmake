
find_path( LOGGING_INCLUDES Logging/ILogger.hpp
    HINTS
        ${CMAKE_PREFIX_PATH}/include
)

# find libraries depending on OS.
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    # handle dll AND lib?
    find_path( LOGGING_SHARED_LIB Logging.dll
        HINTS
            ${CMAKE_PREFIX_PATH}/bin
    )
    find_library( LOGGING_LIB Logging
        HINTS
            ${CMAKE_PREFIX_PATH}/lib
    )
else()
    find_library( LOGGING_SHARED_LIB Logging
        HINTS
            ${CMAKE_PREFIX_PATH}/lib
    )
endif()

if( NOT LOGGING_INCLUDES OR NOT LOGGING_SHARED_LIB )
    set( LOGGING_FOUND FALSE )
else()
    set( LOGGING_FOUND TRUE )
endif()

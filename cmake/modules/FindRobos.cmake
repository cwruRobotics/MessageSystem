find_path( ROBOS_INCLUDE_DIRS Robos/NodeBase.hpp
    HINTS
        ${CMAKE_PREFIX_PATH}/include
)

# find libraries depending on OS.
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    # handle dll AND lib?
    find_path( ROBOS_SHARED_LIB Robos.dll
        HINTS
            ${CMAKE_PREFIX_PATH}/bin
    )
    find_library( ROBOS_LIB Robos
        HINTS
            ${CMAKE_PREFIX_PATH}/lib
    )
else()
    find_library( ROBOS_SHARED_LIB Robos
        HINTS
            ${CMAKE_PREFIX_PATH}/lib
    )
endif()

if( NOT ROBOS_INCLUDE_DIRS OR NOT ROBOS_SHARED_LIB )
    set( ROBOS_FOUND FALSE )
else()
    # set( ROBOS_SHARED_LIB ${ROBOS_SHARED_LIB}/Robos.dll )
    set( ROBOS_FOUND TRUE )
endif()


message( "INC_ROOT: ${INC_ROOT}" )

find_path( CATCH_INCLUDE_DIRS catch/catch.hpp
    HINTS
        ${INC_ROOT}
        ${CATCH_ROOT}
)

message( "CATCH_INCLUDE_DIRS: ${CATCH_INCLUDE_DIRS}" )

if( NOT CATCH_INCLUDE_DIRS )
    set( CATCH_FOUND FALSE )
else()
    set( CATCH_FOUND TRUE )
endif()

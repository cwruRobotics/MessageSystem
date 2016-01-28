
find_path( CATCH_INCLUDE_DIRS catch/catch.hpp
    HINTS
        ${INC_ROOT}
        ${CATCH_ROOT}
)

if( NOT CATCH_INCLUDE_DIRS )
    set( CATCH_FOUND FALSE )
else()
    set( CATCH_FOUND TRUE )
endif()

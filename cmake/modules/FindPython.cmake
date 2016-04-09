
find_library( PYTHON_LIB python${PYTHON_VERSION}
    HINTS
        ${PYTHON_PATH}/libs
)

if( NOT PYTHON_LIB )
    set( PYTHON_FOUND FALSE )
else()
    set( PYTHON_FOUND TRUE )
endif()

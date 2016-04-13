if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    find_library( PYTHON_LIB python${PYTHON_VERSION}
        HINTS
            ${PYTHON_PATH}/libs
    )
    find_path( PYTHON_INCLUDES Python.h
        HINTS
            ${PYTHON_PATH}/include
    )
else()
    find_package( PythonLibs REQUIRED )
    if( PYTHONLIBS_FOUND )
        set( PYTHON_LIB ${PYTHON_LIBRARIES} )
        set( PYTHON_INCLUDES ${PYTHON_INCLUDE_DIRS} )
    else()
        message( FATAL_ERROR "Python not found" )
    endif()
endif()

if( NOT PYTHON_LIB OR NOT PYTHON_INCLUDES )
    set( PYTHON_FOUND FALSE )
else()
    set( PYTHON_FOUND TRUE )
endif()

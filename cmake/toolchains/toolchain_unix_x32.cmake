

if( ${PROCESSOR} MATCHES "aarch64" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "c++ flags" )
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "c flags" )
else()
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32" CACHE STRING "c++ flags" )
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32" CACHE STRING "c flags" )
endif()

set( CMAKE_CXX_COMPILER "g++" )
set( CMAKE_C_COMPILER "gcc" )

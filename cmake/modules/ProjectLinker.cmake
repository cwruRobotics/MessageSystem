
function(LinkProjects Requirement )
    set( ${PROJECT_NAME}_IMPORTED_LIBS )
    foreach(ProjectToLink ${ARGN})
        string( TOUPPER ${ProjectToLink} ProjectToLinkUpper )
        find_package(${ProjectToLink} ${Requirement} )

        # if the library was found. We assume that
        if( ${ProjectToLinkUpper}_FOUND )

            # add an imported library.
            add_library( ${ProjectToLinkUpper}_LIB_VAR SHARED IMPORTED)

            # will set .so for unix systems and .dll for windows
            set_property( TARGET ${ProjectToLinkUpper}_LIB_VAR PROPERTY
                          IMPORTED_LOCATION ${${ProjectToLinkUpper}_SHARED_LIB} )

            # need to link to .lib files for windows
            if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
                set_property( TARGET ${ProjectToLinkUpper}_LIB_VAR PROPERTY
                              IMPORTED_IMPLIB ${${ProjectToLinkUpper}_LIB} )
                # message("${ProjectToLink} IMPORTED_LIBRARY: ${${ProjectToLinkUpper}_SHARED_LIB}")
                # message("${ProjectToLink} STATIC LIBRARY: ${${ProjectToLinkUpper}_LIB}")
            endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

            list( APPEND ${PROJECT_NAME}_IMPORTED_LIBS ${ProjectToLinkUpper}_LIB_VAR )
        else()
            message( FATAL_ERROR "${ProjectToLink} not found" )
        endif()
    endforeach()

    set( ${PROJECT_NAME}_IMPORTED_LIBS ${${PROJECT_NAME}_IMPORTED_LIBS} PARENT_SCOPE )
endfunction()

set( ${PROJECT_NAME}_IMPORTED_LIBS} )
LinkProjects( REQUIRED ${${PROJECT_NAME}_IMPORTED_LIST} )

set( DEPENDENCY_CHECK TRUE )
list( LENGTH ${PROJECT_NAME}_IMPORTED_LIBS ${PROJECT_NAME}_IMPORTED_LIBS_LENGTH )


function( importGTest )
    # Enable ExternalProject CMake module
    include( ExternalProject )

    # Download and install GoogleTest
    ExternalProject_Add(
        gtest
        URL     https://googletest.googlecode.com/files/gtest-1.7.0.zip
        PREFIX  ${CMAKE_CURRENT_BINARY_DIR}/gtest

        # Disable install step
        INSTALL_COMMAND ""
    )

    add_library( libgtest IMPORTED STATIC GLOBAL )
    add_dependencies( libgtest gtest )

    # Set gtest properties
    ExternalProject_Get_Property( gtest source_dir binary_dir )
    set_target_properties( libgtest PROPERTIES
        "IMPORTED_LOCATION" "${binary_dir}/libgtest.lib"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    )
    include_directories( "${source_dir}/include" )

    # Download and install GoogleMock
    ExternalProject_Add(
        gmock
        URL     https://googlemock.googlecode.com/files/gmock-1.7.0.zip
        PREFIX  ${CMAKE_CURRENT_BINARY_DIR}/gmock

        # Disable install step
        INSTALL_COMMAND ""
    )

    add_library( libgmock IMPORTED STATIC GLOBAL )
    add_dependencies( libgmock gmock )

    ExternalProject_Get_Property( gmock source_dir binary_dir )
    set_target_properties( libgmock PROPERTIES 
        "IMPORTED_LOCATION" "${binary_dir}/libgmock.lib"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    )
    include_directories( "${source_dir}/include" )

endfunction()
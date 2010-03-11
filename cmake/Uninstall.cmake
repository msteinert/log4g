#                                          -*- Cmake -*-
# Process this file with cmake to produce build scripts.

# Generate the uninstall script
configure_file(
    # NOTE: change path "cmake/cmake_uninstall.cmake.in" if this script moved
    cmake/cmake_uninstall.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    IMMEDIATE @ONLY
)
# Add the "make uninstall" command
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

#                                          -*- Cmake -*-
# Process this file with cmake to produce build scripts.

find_package(Doxygen)
if(DOXYGEN_FOUND)
    # add doc target
    add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    # set dot variable
    if(DOXYGEN_DOT_EXECUTABLE)
        set(HAVE_DOT YES)
        set(DOT ${DOXYGEN_DOT_EXECUTABLE})
    else(DOXYGEN_DOT_EXECUTABLE)
        set(HAVE_DOT NO)
    endif(DOXYGEN_DOT_EXECUTABLE)
    # configure Doxyfile
    configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
endif(DOXYGEN_FOUND)

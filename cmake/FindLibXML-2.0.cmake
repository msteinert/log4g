#                                          -*- Cmake -*-
# Process this file with cmake to produce build scripts.

# Try to find LibXML-2.0
# Once done, this will define:
# LibXML-2.0_FOUND - system has LibXML-2.0
# LibXML-2.0_INCLUDE_DIRS - the LibXML-2.0 include directories
# LibXML-2.0_LIBRARIES - link these to use LibXML-2.0
include(LibFindMacros)
# use pkg-config for hints
libfind_pkg_check_modules(LibXML-2.0_PKGCONF libxml-2.0)
# find the header file(s)
find_path(LibXML-2.0_INCLUDE_DIR
    NAMES libxml/parser.h
    PATHS
    ${SEARCH_PATH}/include/libxml2
    ${LibXML-2.0_PKGCONF_INCLUDE_DIRS}
)
# find the library
find_library(LibXML-2.0_LIBRARY
    NAMES xml2
    PATHS
    ${SEARCH_PATH}/lib
    ${LibXML-2.0_PKGCONF_LIBRARY_DIRS}
)
# set the include and library variables
set(LibXML-2.0_PROCESS_INCLUDES
    LibXML-2.0_INCLUDE_DIR
)
set(LibXML-2.0_PROCESS_LIBS
    LibXML-2.0_LIBRARY
)
# let libfind_process do the rest
libfind_process(LibXML-2.0)

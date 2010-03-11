#                                          -*- Cmake -*-
# Process this file with cmake to produce build scripts.

# Try to find Log4g
# Once done, this will define:
# Log4g_FOUND - system has Log4g
# Log4g_INCLUDE_DIRS - the Log4g include directories
# Log4g_LIBRARIES - link these to use Log4g
include(LibFindMacros)
# find dependencies
libfind_package(Log4g GObject-2.0)
libfind_package(Log4g LibXML-2.0)
# use pkg-config for hints
libfind_pkg_check_modules(Log4g_PKGCONF gthread-2.0)
# find the header file(s)
find_path(Log4g_INCLUDE_DIR
    NAMES gthread/gthread.h
    PATHS
    ${SEARCH_PATH}/include/glib-2.0
    ${Log4g_PKGCONF_INCLUDE_DIRS}
)
# find the library
find_library(Log4g_LIBRARY
    NAMES gthread-2.0
    PATHS
    ${SEARCH_PATH}/lib
    ${Log4g_PKGCONF_LIBRARY_DIRS}
)
# set the include and library variables
set(Log4g_PROCESS_INCLUDES
    GObject-2.0_INCLUDE_DIRS
    LibXML-2.0_INCLUDE_DIRS
    Log4g_INCLUDE_DIR
)
set(Log4g_PROCESS_LIBS
    GObject-2.0_LIBRARIES
    LibXML-2.0_LIBRARIES
    Log4g_LIBRARY
)
# let libfind_process do the rest
libfind_process(Log4g)

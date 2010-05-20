#                                          -*- Cmake -*-
# Process this file with cmake to produce build scripts.

# Try to find JSONGLib-1.0
# Once done, this will define:
# JSONGLib-1.0_FOUND - system has JSONGLib-1.0
# JSONGLib-1.0_INCLUDE_DIRS - the JSONGLib-1.0 include directories
# JSONGLib-1.0_LIBRARIES - link these to use JSONGLib-1.0
include(LibFindMacros)
# find dependencies
libfind_package(JSONGLib-1.0 GObject-2.0)
# use pkg-config for hints
libfind_pkg_check_modules(JSONGLib-1.0_PKGCONF json-glib-1.0)
# find the header file(s)
find_path(JSONGLib-1.0_INCLUDE_DIR
    NAMES json-glib/json-glib.h
    PATHS
    ${SEARCH_PATH}/include/json-glib-1.0
    ${JSONGLib-1.0_PKGCONF_INCLUDE_DIRS}
)
# find the library
find_library(JSONGLib-1.0_LIBRARY
    NAMES json-glib-1.0
    PATHS
    ${SEARCH_PATH}/lib
    ${JSONGLib-1.0_PKGCONF_LIBRARY_DIRS}
)
# set the include and library variables
set(JSONGLib-1.0_PROCESS_INCLUDES
    GObject-2.0_INCLUDE_DIRS
    JSONGLib-1.0_INCLUDE_DIR
)
set(JSONGLib-1.0_PROCESS_LIBS
    GObject-2.0_LIBRARIES
    JSONGLib-1.0_LIBRARY
)
# let libfind_process do the rest
libfind_process(JSONGLib-1.0)

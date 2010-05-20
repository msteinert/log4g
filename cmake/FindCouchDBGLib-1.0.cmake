#                                          -*- Cmake -*-
# Process this file with cmake to produce build scripts.

# Try to find CouchDBGLib-1.0
# Once done, this will define:
# CouchDBGLib-1.0_FOUND - system has CouchDBGLib-1.0
# CouchDBGLib-1.0_INCLUDE_DIRS - the CouchDBGLib-1.0 include directories
# CouchDBGLib-1.0_LIBRARIES - link these to use CouchDBGLib-1.0
include(LibFindMacros)
# find dependencies
libfind_package(CouchDBGLib-1.0 GObject-2.0)
libfind_package(CouchDBGLib-1.0 JSONGLib-1.0)
# use pkg-config for hints
libfind_pkg_check_modules(CouchDBGLib-1.0_PKGCONF couchdb-glib-1.0)
# find the header file(s)
find_path(CouchDBGLib-1.0_INCLUDE_DIR
    NAMES couchdb-glib.h
    PATHS
    ${SEARCH_PATH}/include/json-glib-1.0
    ${CouchDBGLib-1.0_PKGCONF_INCLUDE_DIRS}
)
# find the library
find_library(CouchDBGLib-1.0_LIBRARY
    NAMES couchdb-glib-1.0
    PATHS
    ${SEARCH_PATH}/lib
    ${CouchDBGLib-1.0_PKGCONF_LIBRARY_DIRS}
)
# set the include and library variables
set(CouchDBGLib-1.0_PROCESS_INCLUDES
    GObject-2.0_INCLUDE_DIRS
    JSONGLib-1.0_INCLUDE_DIRS
    CouchDBGLib-1.0_INCLUDE_DIR
)
set(CouchDBGLib-1.0_PROCESS_LIBS
    GObject-2.0_LIBRARIES
    JSONGLib-1.0_LIBRARIES
    CouchDBGLib-1.0_LIBRARY
)
# let libfind_process do the rest
libfind_process(CouchDBGLib-1.0)

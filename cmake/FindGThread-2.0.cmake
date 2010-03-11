#                                          -*- Cmake -*-
# Process this file with cmake to produce build scripts.

# Try to find GThread-2.0
# Once done, this will define:
# GThread-2.0_FOUND - system has GThread-2.0
# GThread-2.0_INCLUDE_DIRS - the GThread-2.0 include directories
# GThread-2.0_LIBRARIES - link these to use GThread-2.0
include(LibFindMacros)
# find dependencies
libfind_package(GThread-2.0 GLib-2.0)
# use pkg-config for hints
libfind_pkg_check_modules(GThread-2.0_PKGCONF gthread-2.0)
# find the header file(s)
find_path(GThread-2.0_INCLUDE_DIR
    NAMES gthread/gthread.h
    PATHS
    ${SEARCH_PATH}/include/glib-2.0
    ${GThread-2.0_PKGCONF_INCLUDE_DIRS}
)
# find the library
find_library(GThread-2.0_LIBRARY
    NAMES gthread-2.0
    PATHS
    ${SEARCH_PATH}/lib
    ${GThread-2.0_PKGCONF_LIBRARY_DIRS}
)
# set the include and library variables
set(GThread-2.0_PROCESS_INCLUDES
    GLib-2.0_INCLUDE_DIRS
    GThread-2.0_INCLUDE_DIR
)
set(GThread-2.0_PROCESS_LIBS
    GLib-2.0_LIBRARIES
    GThread-2.0_LIBRARY
)
# let libfind_process do the rest
libfind_process(GThread-2.0)

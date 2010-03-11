#                                          -*- Cmake -*-
# Process this file with cmake to produce build scripts.

# Try to find GLib-2.0
# Once done, this will define:
# GLib-2.0_FOUND - system has GLib-2.0
# GLib-2.0_INCLUDE_DIRS - the GLib-2.0 include directories
# GLib-2.0_LIBRARIES - link these to use GLib-2.0
include(LibFindMacros)
# use pkg-config for hints
libfind_pkg_check_modules(GLib-2.0_PKGCONF glib-2.0)
# find the header file(s)
find_path(GLib-2.0_INCLUDE_DIR
    NAMES glib.h
    PATHS
    ${SEARCH_PATH}/include/glib-2.0
    ${GLib-2.0_PKGCONF_INCLUDE_DIRS}
)
find_path(GLibConfig-2.0_INCLUDE_DIR
    NAMES glibconfig.h
    PATHS
    ${SEARCH_PATH}/lib/glib-2.0/include
    ${GLib-2.0_PKGCONF_INCLUDE_DIRS}
)
# find the library
find_library(GLib-2.0_LIBRARY
    NAMES glib-2.0
    PATHS
    ${SEARCH_PATH}/lib
    ${GLib-2.0_PKGCONF_LIBRARY_DIRS}
)
# find glib-genmarshal
find_program(GLib-2.0_GENMARSHAL glib-genmarshal
    PATHS ${SEARCH_PATH}/bin
)
# find gtester
find_program(GLib-2.0_GTESTER gtester
    PATHS ${SEARCH_PATH}/bin
)
# find gtester-report
find_program(GLib-2.0_GTESTER_REPORT gtester-report
    PATHS ${SEARCH_PATH}/bin
)
# set the include and library variables
set(GLib-2.0_PROCESS_INCLUDES
    GLib-2.0_INCLUDE_DIR
    GLibConfig-2.0_INCLUDE_DIR
)
set(GLib-2.0_PROCESS_LIBS
    GLib-2.0_LIBRARY
)
# let libfind_process do the rest
libfind_process(GLib-2.0)

#                                          -*- Cmake -*-
# Process this file with cmake to produce build scripts.

# Try to find GObject-2.0
# Once done, this will define:
# GObject-2.0_FOUND - system has GObject-2.0
# GObject-2.0_INCLUDE_DIRS - the GObject-2.0 include directories
# GObject-2.0_LIBRARIES - link these to use GObject-2.0
include(LibFindMacros)
# find dependencies
libfind_package(GObject-2.0 GLib-2.0)
# use pkg-config for hints
libfind_pkg_check_modules(GObject-2.0_PKGCONF gobject-2.0)
# find the header file(s)
find_path(GObject-2.0_INCLUDE_DIR
    NAMES glib-object.h
    PATHS
    ${SEARCH_PATH}/include/glib-2.0
    ${GObject-2.0_PKGCONF_INCLUDE_DIRS}
)
# find the library
find_library(GObject-2.0_LIBRARY
    NAMES gobject-2.0
    PATHS
    ${SEARCH_PATH}/lib
    ${GObject-2.0_PKGCONF_LIBRARY_DIRS}
)
# set the include and library variables
set(GObject-2.0_PROCESS_INCLUDES
    GLib-2.0_INCLUDE_DIRS
    GObject-2.0_INCLUDE_DIR
)
set(GObject-2.0_PROCESS_LIBS
    GLib-2.0_LIBRARIES
    GObject-2.0_LIBRARY
)
# let libfind_process do the rest
libfind_process(GObject-2.0)

#                                          -*- Cmake -*-
# Process this file with cmake to produce build scripts.

# Try to find GModule-2.0
# Once done, this will define:
# GModule-2.0_FOUND - system has GModule-2.0
# GModule-2.0_INCLUDE_DIRS - the GModule-2.0 include directories
# GModule-2.0_LIBRARIES - link these to use GModule-2.0
include(LibFindMacros)
# find dependencies
libfind_package(GModule-2.0 GLib-2.0)
# use pkg-config for hints
libfind_pkg_check_modules(GModule-2.0_PKGCONF gobject-2.0)
# find the header file(s)
find_path(GModule-2.0_INCLUDE_DIR
    NAMES gmodule.h
    PATHS
    ${SEARCH_PATH}/include/glib-2.0
    ${GModule-2.0_PKGCONF_INCLUDE_DIRS}
)
# find the library
find_library(GModule-2.0_LIBRARY
    NAMES gmodule-2.0
    PATHS
    ${SEARCH_PATH}/lib
    ${GModule-2.0_PKGCONF_LIBRARY_DIRS}
)
# set the include and library variables
set(GModule-2.0_PROCESS_INCLUDES
    GLib-2.0_INCLUDE_DIRS
    GModule-2.0_INCLUDE_DIR
)
set(GModule-2.0_PROCESS_LIBS
    GLib-2.0_LIBRARIES
    GModule-2.0_LIBRARY
)
# let libfind_process do the rest
libfind_process(GModule-2.0)
if(GModule-2.0_FOUND)
    set(GModule-2.0_LIBRARIES ${GModule-2.0_LIBRARIES} -dl)
endif(GModule-2.0_FOUND)

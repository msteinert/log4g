#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Initialize autoconf
AC_PREREQ(2.59)
AC_INIT([log4g], [0.1.1], [mike.steinert@gmail.com])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/log4g.c])
# Checks for programs
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LIBTOOL
DOLT
# Check for Doxygen
AC_PATH_PROG([DOXYGEN], [doxygen])
AC_CACHE_VAL([log4g_cv_HAVE_DOT],
    [AC_PATH_PROG([DOT], [dot])
    if test "x$DOT" != "x"; then
        log4g_cv_HAVE_DOT=YES
    else
        log4g_cv_HAVE_DOT=NO
    fi])
HAVE_DOT="$log4g_cv_HAVE_DOT"
AC_SUBST([HAVE_DOT])
# Checks for dependencies
PKG_PROG_PKG_CONFIG([0.17])
log4g_glib_version="glib-2.0 >= 2.20"
AC_SUBST([log4g_glib_version])
PKG_CHECK_MODULES([GLIB], [$log4g_glib_version])
log4g_gmodule_version="gmodule-2.0 >= 2.20"
AC_SUBST([log4g_gmodule_version])
PKG_CHECK_MODULES([GMODULE], [$log4g_gmodule_version])
log4g_gobject_version="gobject-2.0 >= 2.20"
AC_SUBST([log4g_gobject_version])
PKG_CHECK_MODULES([GOBJECT], [$log4g_gobject_version])
log4g_gthread_version="gthread-2.0 >= 2.20"
AC_SUBST([log4g_gthread_version])
PKG_CHECK_MODULES([GTHREAD], [$log4g_gthread_version])
log4g_libxml_version="libxml-2.0 >= 2.6"
AC_SUBST([log4g_libxml_version])
PKG_CHECK_MODULES([LIBXML2], [$log4g_libxml_version])
# Add configuration
AC_DEFINE([ENABLE_NLS], [1],
    [Always defined to indicate that i18n is enabled])
AC_DEFINE_UNQUOTED([G_LOG_DOMAIN], ["$PACKAGE_NAME"],
    [The log domain for this project])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$PACKAGE_NAME"],
    [The i18n domain for this project])
AC_CHECK_FUNCS([bind_textdomain_codeset])
AH_BOTTOM(
[/* Include GLib i18n header file */
#include <glib/gi18n-lib.h>

/* Include G_DEFINE_INTERFACE definition */
#include <glib.h>
#ifndef G_DEFINE_INTERFACE
#include "support/glib-2.23/ginterface.h"
#endif

/* Include internal logging API */
#include "log4g/helpers/log.h"
])dnl
# Output files
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    doc/Doxyfile
    doc/Makefile
    Makefile
    resources/log4g.pc
    src/Makefile
    tests/Makefile
])
AC_OUTPUT

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Initialize autoconf
AC_PREREQ(2.59)
AC_INIT([log4g], [0.1], [mike.steinert@gmail.com])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/log4g.c])
# Checks for programs
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LIBTOOL
DOLT
# check for Doxygen
AC_PATH_PROG(DOXYGEN, doxygen)
AC_CACHE_VAL(log4g_cv_HAVE_DOT,
    [AC_PATH_PROG(DOT, dot)
    if test "x$DOT" != "x"; then
        log4g_cv_HAVE_DOT=YES
    else
        log4g_cv_HAVE_DOT=NO
    fi])
HAVE_DOT="$log4g_cv_HAVE_DOT"
AC_SUBST(HAVE_DOT)
# Checks for dependencies
PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.20)
PKG_CHECK_MODULES(GMODULE, gmodule-2.0 >= 2.20)
PKG_CHECK_MODULES(GOBJECT, gobject-2.0 >= 2.20)
PKG_CHECK_MODULES(GTHREAD, gthread-2.0 >= 2.20)
PKG_CHECK_MODULES(LIBXML2, libxml-2.0 >= 2.6)
# add configuration
AC_DEFINE([ENABLE_NLS], [1],
    [Always defined to indicate that i18n is enabled])
AC_CHECK_FUNCS(bind_textdomain_codeset)
AH_BOTTOM(
[/* Define to the location where the catalogs will be installed */
#define LOG4G_LOCALEDIR "@prefix@/share/locale"

/* The log domain for this project */
#define G_LOG_DOMAIN "@PROJECT_NAME@"

/* The i18n domain for this project */
#define GETTEXT_PACKAGE "@PROJECT_NAME@"
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

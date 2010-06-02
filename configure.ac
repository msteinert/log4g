#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Initialize autoconf
AC_PREREQ([2.64])
# Library versioning
# Making releases:
#   log4g_micro_version += 1
#   log4g_interface_age += 1
# If functions have been added or backward-compatibility has been broken:
#   log4g_interface_age = 0
m4_define([log4g_major_version], [0])
m4_define([log4g_minor_version], [1])
m4_define([log4g_micro_version], [2])
m4_define([log4g_interface_age], [0])
# Initialize autoconf & automake
AC_INIT([Log4g], [log4g_major_version.log4g_minor_version.log4g_micro_version],
    [http://github.com/msteinert/log4g/issues/], [log4g],
    [http://msteinert.github.com/log4g/])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wno-portability])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_CONFIG_SRCDIR([log4g/log4g.c])
# Checks for programs
AC_ENABLE_SHARED
AC_DISABLE_STATIC
AC_PROG_CC_C99
AC_PROG_LIBTOOL
GOBJECT_INTROSPECTION_CHECK([0.6.10])
if test "x$GCC" = "xyes"; then
    case " $CFLAGS " in
    *[[\ \	]]-Wall[[\ \	]]*) ;;
    *) CFLAGS="$CFLAGS -Wall" ;;
    esac
fi
# Pkg-config versioning
log4g_series_major=1
log4g_series_minor=0
log4g_series=$log4g_series_major.$log4g_series_minor
AC_SUBST([log4g_series_major])
AC_SUBST([log4g_series_minor])
AC_SUBST([log4g_series])
# Libtool versioning
LT_CURRENT=m4_eval(100 * log4g_minor_version + log4g_micro_version - log4g_interface_age)
LT_REVISION=log4g_interface_age
LT_AGE=m4_eval(m4_eval(100 * log4g_minor_version + log4g_micro_version) - log4g_interface_age)
AC_SUBST([LT_CURRENT])
AC_SUBST([LT_REVISION])
AC_SUBST([LT_AGE])
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
log4g_glib_minimum_version=2.24
log4g_libxml_minimum_version=2.6
PKG_PROG_PKG_CONFIG([0.17])
log4g_glib_version="glib-2.0 >= $log4g_glib_minimum_version"
AC_SUBST([log4g_glib_version])
PKG_CHECK_MODULES([GLIB], [$log4g_glib_version])
log4g_gmodule_version="gmodule-2.0 >= $log4g_glib_minimum_version"
AC_SUBST([log4g_gmodule_version])
PKG_CHECK_MODULES([GMODULE], [$log4g_gmodule_version])
log4g_gobject_version="gobject-2.0 >= $log4g_glib_minimum_version"
AC_SUBST([log4g_gobject_version])
PKG_CHECK_MODULES([GOBJECT], [$log4g_gobject_version])
log4g_gthread_version="gthread-2.0 >= $log4g_glib_minimum_version"
AC_SUBST([log4g_gthread_version])
PKG_CHECK_MODULES([GTHREAD], [$log4g_gthread_version])
log4g_libxml_version="libxml-2.0 >= $log4g_libxml_minimum_version"
AC_SUBST([log4g_libxml_version])
PKG_CHECK_MODULES([LIBXML2], [$log4g_libxml_version])
# Appenders option
AC_ARG_WITH(appenders,
    [AC_HELP_STRING([--with-appenders],
        [enable core appenders @<:@default=yes@:>@])],,
    [withval=yes])
AC_CACHE_CHECK([for core appender support], [log4g_cv_with_appenders],
        [log4g_cv_with_appenders="$withval"])
if test "x$log4g_cv_with_appenders" != "xno"; then
    LOG4G_CONFIGURED_MODULES="$LOG4G_CONFIGURED_MODULES appenders"
fi
AM_CONDITIONAL([LOG4G_WITH_APPENDERS],
    [test "x$log4g_cv_with_appenders" = "xyes"])
# Filters option
AC_ARG_WITH(filters,
    [AC_HELP_STRING([--with-filters],
        [enable core filters @<:@default=yes@:>@])],,
    [withval=yes])
AC_CACHE_CHECK([for core filter support], [log4g_cv_with_filters],
        [log4g_cv_with_filters="$withval"])
if test "x$log4g_cv_with_filters" != "xno"; then
    LOG4G_CONFIGURED_MODULES="$LOG4G_CONFIGURED_MODULES filters"
fi
AM_CONDITIONAL([LOG4G_WITH_FILTERS], [test "x$log4g_cv_with_filters" = "xyes"])
# Layouts option
AC_ARG_WITH(layouts,
    [AC_HELP_STRING([--with-layouts],
        [enable core layouts @<:@default=yes@:>@])],,
    [withval=yes])
AC_CACHE_CHECK([for core layout support], [log4g_cv_with_layouts],
        [log4g_cv_with_layouts="$withval"])
if test "x$log4g_cv_with_layouts" != "xno"; then
    LOG4G_CONFIGURED_MODULES="$LOG4G_CONFIGURED_MODULES layouts"
fi
AM_CONDITIONAL([LOG4G_WITH_LAYOUTS], [test "x$log4g_cv_with_layouts" = "xyes"])
# CouchDB option
AC_ARG_WITH(couchdb,
    [AC_HELP_STRING([--with-couchdb],
        [enable CouchDB appender & layout @<:@default=auto@:>@])],,
    [withval=auto])
AC_CACHE_CHECK([for CouchDB support], [log4g_cv_with_couchdb],
        [log4g_cv_with_couchdb="$withval"])
if test "x$log4g_cv_with_couchdb" != "xno"; then
    PKG_CHECK_MODULES([COUCHDB_GLIB], ["couchdb-glib-1.0"],
        [LOG4G_CONFIGURED_MODULES="$LOG4G_CONFIGURED_MODULES couchdb"
        log4g_with_couchdb=yes],
        [log4g_with_couchdb=no])
fi
AM_CONDITIONAL([LOG4G_WITH_COUCHDB], [test "x$log4g_with_couchdb" = "xyes"])
# Configure module options
AC_SUBST([LOG4G_CONFIGURED_MODULES])
# Configure module directory
moduledir="\$(libdir)/log4g-$log4g_series"
AC_SUBST(moduledir)
# Add configuration
AC_DEFINE([ENABLE_NLS], [1],
    [Always defined to indicate that i18n is enabled])
AC_DEFINE_UNQUOTED([G_LOG_DOMAIN], ["$PACKAGE_TARNAME"],
    [The log domain for this project])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$PACKAGE_TARNAME"],
    [The i18n domain for this project])
AC_CHECK_FUNCS([bind_textdomain_codeset])
AH_BOTTOM(
[/* Include GLib i18n header file */
#include <glib/gi18n-lib.h>

/* Include internal logging API */
#include "log4g/helpers/log.h"
])dnl
# Output files
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    doc/Doxyfile
    doc/Makefile
    log4g/Makefile
    Makefile
    modules/Makefile
    modules/appenders/Makefile
    modules/couchdb/Makefile
    modules/filters/Makefile
    modules/layouts/Makefile
    resources/log4g.pc
    tests/Makefile
])
AC_OUTPUT
AC_MSG_RESULT([
Log4g build options:
	Version: $VERSION
	Modules:$LOG4G_CONFIGURED_MODULES
	Introspection: $found_introspection
])

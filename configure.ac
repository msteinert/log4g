#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Initialize autoconf
AC_PREREQ([2.64])
# Library versioning
# Making releases:
#   micro_version += 1
#   interface_age += 1
# If functions have been added or backward-compatibility has been broken:
#   interface_age = 0
m4_define([major_version], [0])
m4_define([minor_version], [1])
m4_define([micro_version], [2])
m4_define([interface_age], [0])
# Initialize autoconf & automake
AC_INIT([Log4g], [major_version.minor_version.micro_version],
	[http://github.com/msteinert/log4g/issues/], [log4g],
	[http://msteinert.github.com/log4g/])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wno-portability])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_CONFIG_SRCDIR([log4g/log4g.h])
# Initialize Libtool
LT_PREREQ([2.2])
LT_INIT([disable-static])
# Checks for programs
m4_ifdef([AC_PROG_CC_C99], [AC_PROG_CC_C99],
	[AC_PROG_CC
	AX_TRY_CFLAGS([-std=c99], [AX_FLAGS([-std=c99])])])
PKG_PROG_PKG_CONFIG([0.17])
# Glib/Gtk stuff
GOBJECT_INTROSPECTION_CHECK([0.6.8])
m4_ifdef([GTK_DOC_CHECK], [
GTK_DOC_CHECK([1.14], [--flavour no-tmpl])
], [
AM_CONDITIONAL([ENABLE_GTK_DOC], false)
])
# Enable compiler flags
AX_TRY_CFLAGS([-Wall], [AX_CFLAGS([-Wall])])
AX_TRY_CFLAGS([-Wstrict-prototypes], [AX_CFLAGS([-Wstrict-prototypes])])
AX_TRY_CFLAGS([-Wcast-align], [AX_CFLAGS([-Wcast-align])])
AX_TRY_CFLAGS([-fno-common], [AX_CFLAGS([-fno-common])])
AX_TRY_CFLAGS([-g], [AX_CFLAGS([-g])])
# Pkg-config versioning
series_major=1
series_minor=0
series=$series_major.$series_minor
AC_SUBST([series_major])
AC_SUBST([series_minor])
AC_SUBST([series])
# Libtool versioning
LT_CURRENT=m4_eval(100 * minor_version + micro_version - interface_age)
LT_REVISION=interface_age
LT_AGE=m4_eval(m4_eval(100 * minor_version + micro_version) - interface_age)
AC_SUBST([LT_CURRENT])
AC_SUBST([LT_REVISION])
AC_SUBST([LT_AGE])
# Checks for dependencies
glib_minimum_version=2.24
glib_version="glib-2.0 >= $glib_minimum_version"
AC_SUBST([glib_version])
PKG_CHECK_MODULES([GLIB], [$glib_version])
gmodule_version="gmodule-2.0 >= $glib_minimum_version"
AC_SUBST([gmodule_version])
PKG_CHECK_MODULES([GMODULE], [$gmodule_version])
gobject_version="gobject-2.0 >= $glib_minimum_version"
AC_SUBST([gobject_version])
PKG_CHECK_MODULES([GOBJECT], [$gobject_version])
gthread_version="gthread-2.0 >= $glib_minimum_version"
AC_SUBST([gthread_version])
PKG_CHECK_MODULES([GTHREAD], [$gthread_version])
libxml_minimum_version=2.6
libxml_version="libxml-2.0 >= $libxml_minimum_version"
AC_SUBST([libxml_version])
PKG_CHECK_MODULES([LIBXML2], [$libxml_version])
# Check for glib-genmarshal
AC_ARG_VAR([GLIB_GENMARSHAL], [path to the glib-genmarshal(1) utility])
AS_IF([test "x$GLIB_GENMARSHAL" = "x"],
	[PKG_CHECK_EXISTS([$glib_version],
		[GLIB_GENMARSHAL=`$PKG_CONFIG --variable=glib_genmarshal $glib_version`])])
# Appenders option
AC_ARG_WITH(appenders,
	[AC_HELP_STRING([--with-appenders],
		[enable core appenders @<:@default=yes@:>@])],,
	[with_appenders=yes])
AS_IF([test "x$with_appenders" != "xno"],
	[LOG4G_CONFIGURED_MODULES="$LOG4G_CONFIGURED_MODULES appenders"
	AC_DEFINE([LOG4G_WITH_APPENDERS], [1],
		[Define to 1 if appenders are enabled])])
AM_CONDITIONAL([LOG4G_WITH_APPENDERS],
	[test "x$with_appenders" != "xno"])
# Filters option
AC_ARG_WITH(filters,
	[AC_HELP_STRING([--with-filters],
		[enable core filters @<:@default=yes@:>@])],,
	[with_filters=yes])
AS_IF([test "x$with_filters" != "xno"],
	[LOG4G_CONFIGURED_MODULES="$LOG4G_CONFIGURED_MODULES filters"
	AC_DEFINE([LOG4G_WITH_FILTERS], [1],
		[Define to 1 if filters are enabled])])
AM_CONDITIONAL([LOG4G_WITH_FILTERS],
	[test "x$with_filters" != "xno"])
# Layouts option
AC_ARG_WITH(layouts,
	[AC_HELP_STRING([--with-layouts],
		[enable core layouts @<:@default=yes@:>@])],,
	[with_layouts=yes])
AS_IF([test "x$with_layouts" != "xno"],
	[LOG4G_CONFIGURED_MODULES="$LOG4G_CONFIGURED_MODULES layouts"
	AC_DEFINE([LOG4G_WITH_LAYOUTS], [1],
		[Define to 1 if Layouts are enabled])])
AM_CONDITIONAL([LOG4G_WITH_LAYOUTS],
	[test "x$with_layouts" != "xno"])
# CouchDB option
AC_ARG_WITH(couchdb,
	[AC_HELP_STRING([--with-couchdb],
		[enable CouchDB appender & layout @<:@default=auto@:>@])],,
	[with_couchdb=auto])
AS_IF([test "x$with_couchdb" != "xno"],
	[PKG_CHECK_MODULES([COUCHDB_GLIB], ["couchdb-glib-1.0"],
		[LOG4G_CONFIGURED_MODULES="$LOG4G_CONFIGURED_MODULES couchdb"
		AC_DEFINE([LOG4G_WITH_COUCHDB], [1],
			[Define to 1 if Couchdb is enabled])
		with_couchdb=yes],
		[with_couchdb=no])])
AM_CONDITIONAL([LOG4G_WITH_COUCHDB],
	[test "x$with_couchdb" != "xno"])
# Configure module options
AC_SUBST([LOG4G_CONFIGURED_MODULES])
# Configure module directory
moduledir="\$(libdir)/log4g-$series"
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
	docs/Makefile
	docs/reference/Makefile
	docs/reference/log4g/Makefile
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
	Host: $host
	Version: $VERSION
	Modules:$LOG4G_CONFIGURED_MODULES
	Introspection: $found_introspection
])

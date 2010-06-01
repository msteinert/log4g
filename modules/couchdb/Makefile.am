#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

module_LTLIBRARIES = \
    liblog4g-couchdb.la
liblog4g_couchdb_la_SOURCES = \
    appender/couchdb-appender.h \
    couchdb-appender.c \
    couchdb-layout.c \
    layout/couchdb-layout.h \
    module.c
liblog4g_couchdb_la_CFLAGS = \
    -std=gnu99 -Wall -Werror \
    -I$(srcdir) -I$(top_srcdir) \
    $(COUCHDB_GLIB_CFLAGS) \
    $(GLIB_CFLAGS) \
    $(GOBJECT_CFLAGS)
liblog4g_couchdb_la_LDFLAGS = \
    -avoid-version -shared \
    $(top_builddir)/log4g/liblog4g-$(log4g_series).la \
    $(COUCHDB_GLIB_LIBS) \
    $(GLIB_LIBS) \
    $(GOBJECT_LIBS)
MAINTAINERCLEANFILES = \
    $(srcdir)/Makefile.in

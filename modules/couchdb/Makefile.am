#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

module_LTLIBRARIES = \
    liblog4g-couchdb.la
liblog4g_couchdb_la_SOURCES = \
    couchdb-appender.c \
    couchdb-appender.h \
    couchdb-layout.c \
    couchdb-layout.h \
    module.c
liblog4g_couchdb_la_CFLAGS = \
    -std=gnu99 -Wall -Werror \
    -I$(top_srcdir) \
    -I$(top_srcdir)/src \
    -I$(top_srcdir)/modules/couchdb \
    $(COUCHDB_GLIB_CFLAGS) \
    $(GLIB_CFLAGS) \
    $(GOBJECT_CFLAGS)
liblog4g_couchdb_la_LDFLAGS = \
    -avoid-version -shared \
    $(COUCHDB_GLIB_LIBS) \
    $(GLIB_LIBS) \
    $(GOBJECT_LIBS)
MAINTAINERCLEANFILES = \
    $(srcdir)/Makefile.in

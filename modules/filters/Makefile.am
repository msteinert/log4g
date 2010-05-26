#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

module_LTLIBRARIES = \
    liblog4g-filters.la
liblog4g_filters_la_SOURCES = \
    deny-all-filter.c \
    deny-all-filter.h \
    level-range-filter.c \
    level-range-filter.h \
    level-match-filter.c \
    level-match-filter.h \
    module.c \
    string-match-filter.c \
    string-match-filter.h
liblog4g_filters_la_CFLAGS = \
    -std=gnu99 -Wall -Werror \
    -I$(top_srcdir) \
    -I$(top_srcdir)/src \
    -I$(top_srcdir)/modules/filters \
    $(GLIB_CFLAGS) \
    $(GOBJECT_CFLAGS)
liblog4g_filters_la_LDFLAGS = \
    -avoid-version -shared \
    $(GLIB_LIBS) \
    $(GOBJECT_LIBS)
MAINTAINERCLEANFILES = Makefile.in

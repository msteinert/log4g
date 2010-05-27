#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

module_LTLIBRARIES = \
    liblog4g-layouts.la
liblog4g_layouts_la_SOURCES = \
    date-layout.c \
    date-layout.h \
    html-layout.c \
    html-layout.h \
    module.c \
    pattern-converter.c \
    pattern-converter.h \
    pattern-layout.c \
    pattern-layout.h \
    pattern-parser.c \
    pattern-parser.h \
    simple-layout.c \
    simple-layout.h \
    ttcc-layout.c \
    ttcc-layout.h \
    xml-layout.c \
    xml-layout.h
liblog4g_layouts_la_CFLAGS = \
    -std=gnu99 -Wall -Werror \
    -I$(top_srcdir) \
    -I$(top_srcdir)/src \
    -I$(top_srcdir)/modules/layouts \
    $(GLIB_CFLAGS) \
    $(GOBJECT_CFLAGS)
liblog4g_layouts_la_LDFLAGS = \
    -avoid-version -shared \
    $(GLIB_LIBS) \
    $(GOBJECT_LIBS)
MAINTAINERCLEANFILES = \
    $(srcdir)/Makefile.in

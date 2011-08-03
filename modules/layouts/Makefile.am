#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

module_LTLIBRARIES = \
	liblog4g-layouts.la
liblog4g_layouts_la_SOURCES = \
	date-layout.c \
	html-layout.c \
	layout/date-layout.h \
	layout/html-layout.h \
	layout/simple-layout.h \
	layout/ttcc-layout.h \
	layout/xml-layout.h \
	module.c \
	pattern-converter.c \
	pattern-layout.c \
	helpers/pattern-converter.h \
	helpers/pattern-parser.h \
	layout/pattern-layout.h \
	pattern-parser.c \
	simple-layout.c \
	ttcc-layout.c \
	xml-layout.c
liblog4g_layouts_la_CFLAGS = \
	-I$(srcdir) -I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS)
liblog4g_layouts_la_LDFLAGS = \
	-avoid-version -shared \
	$(top_builddir)/log4g/liblog4g-$(series).la \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS)

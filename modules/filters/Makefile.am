#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

module_LTLIBRARIES = \
	liblog4g-filters.la
liblog4g_filters_la_SOURCES = \
	deny-all-filter.c \
	filter/deny-all-filter.h \
	filter/level-range-filter.h \
	filter/level-match-filter.h \
	filter/string-match-filter.h \
	level-range-filter.c \
	level-match-filter.c \
	module.c \
	string-match-filter.c
liblog4g_filters_la_CFLAGS = \
	-I$(srcdir) -I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS)
liblog4g_filters_la_LDFLAGS = \
	-avoid-version -shared \
	$(top_builddir)/log4g/liblog4g-$(log4g_series).la \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS)

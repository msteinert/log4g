#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# generate marshalling routines
$(srcdir)/marshal.h: marshal.list
	$(AM_V_GEN)$(GLIB_GENMARSHAL) --prefix=log4g_marshal --header $< > $@
$(srcdir)/marshal.c: marshal.list
	$(AM_V_GEN)$(GLIB_GENMARSHAL) --prefix=log4g_marshal --body $< > $@
# library definition
lib_LTLIBRARIES = liblog4g-@series_major@.@series_minor@.la
liblog4g_@series_major@_@series_minor@_la_SOURCES = \
	appender.c \
	appender-attachable.c \
	appender-attachable-impl.c \
	basic-configurator.c \
	configurator.c \
	default-logger-factory.c \
	default-module-loader.c \
	default-repository-selector.c \
	dom-configurator.c \
	error.c \
	error-handler.c \
	filter.c \
	hierarchy.c \
	layout.c \
	level.c \
	log-manager.c \
	log.c \
	helpers/log.h \
	log4g.c \
	logger.c \
	logger-factory.c \
	logger-repository.c \
	logging-event.c \
	mdc.c \
	module.c \
	module-loader.c \
	ndc.c \
	only-once-error-handler.c \
	provision-node.c \
	repository-selector.c \
	root-logger.c \
	marshal.c \
	marshal.h \
	thread.c
BUILT_SOURCES = \
	marshal.c \
	marshal.h
# include directories & compiler switches
liblog4g_@series_major@_@series_minor@_la_INCLUDES = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GMODULE_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(LIBXML2_CFLAGS)
liblog4g_@series_major@_@series_minor@_la_CFLAGS = \
	-DLOG4G_LOCALEDIR=\"$(localedir)\" \
	-DLOG4G_MODULEDIR=\"$(moduledir)\" \
	$(liblog4g_@series_major@_@series_minor@_la_INCLUDES)
liblog4g_@series_major@_@series_minor@_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	$(GLIB_LIBS) \
	$(GMODULE_LIBS) \
	$(GOBJECT_LIBS) \
	$(LIBXML2_LIBS)
# install development headers
pkgincludedir = $(includedir)/log4g-$(series)/log4g
pkginclude_HEADERS = \
	appender.h \
	basic-configurator.h \
	dom-configurator.h \
	error.h \
	filter.h \
	hierarchy.h \
	layout.h \
	level.h \
	log4g.h \
	log-manager.h \
	logger.h \
	logging-event.h \
	mdc.h \
	module.h \
	ndc.h \
	provision-node.h \
	root-logger.h
disableincludedir = $(pkgincludedir)
disableinclude_HEADERS = \
	disable.h
helperincludedir = $(includedir)/log4g-$(series)/log4g/helpers
helperinclude_HEADERS = \
	helpers/appender-attachable-impl.h \
	helpers/default-logger-factory.h \
	helpers/default-module-loader.h \
	helpers/default-repository-selector.h \
	helpers/only-once-error-handler.h \
	helpers/thread.h
interfaceincludedir = $(includedir)/log4g-$(series)/log4g/interface
interfaceinclude_HEADERS = \
	interface/appender-attachable.h \
	interface/configurator.h \
	interface/error-handler.h \
	interface/logger-factory.h \
	interface/logger-repository.h \
	interface/module-loader.h \
	interface/repository-selector.h
EXTRA_DIST = \
	marshal.list
-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = \
	--add-include-path=$(srcdir)
INTROSPECTION_COMPILER_ARGS = \
	--includedir=$(srcdir)
if HAVE_INTROSPECTION
introspection_files = \
	$(liblog4g_@series_major@_@series_minor@_la_SOURCES) \
	$(pkginclude_HEADERS) \
	$(helperinclude_HEADERS) \
	$(interfaceinclude_HEADERS)
Log4g-$(series).gir: $(INTROSPECTION_SCANNER) liblog4g-$(series).la Makefile
Log4g_@series_major@_@series_minor@_gir_SCANNERFLAGS = \
	--strip-prefix=Log4g
Log4g_@series_major@_@series_minor@_gir_INCLUDES = \
	GObject-2.0 GModule-2.0 libxml2-2.0
Log4g_@series_major@_@series_minor@_gir_CFLAGS = \
	$(liblog4g_@series_major@_@series_minor@_la_INCLUDES)
Log4g_@series_major@_@series_minor@_gir_LIBS = \
	liblog4g-$(series).la
Log4g_@series_major@_@series_minor@_gir_FILES = \
	$(introspection_files:%=$(srcdir)/%)
INTROSPECTION_GIRS += Log4g-$(series).gir
girdir = $(datadir)/gir-1.0
gir_DATA = Log4g-$(series).gir
typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)
Log4g-$(series).gir: liblog4g-@series_major@.@series_minor@.la
CLEANFILES = $(gir_DATA) $(typelib_DATA)
endif

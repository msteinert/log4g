#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# generate marshalling routines
support/marshal.h: support/marshal.list
	glib-genmarshal --header $< > $@
support/marshal.c: support/marshal.list
	glib-genmarshal --body $< > $@
# library definition
lib_LTLIBRARIES = liblog4g-@log4g_series_major@.@log4g_series_minor@.la
liblog4g_@log4g_series_major@_@log4g_series_minor@_la_SOURCES = \
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
    support/marshal.c \
    support/marshal.h \
    thread.c
# include directories & compiler switches
liblog4g_@log4g_series_major@_@log4g_series_minor@_la_CFLAGS = \
    -std=gnu99 -Wall -Werror \
    -DLOG4G_LOCALEDIR=\"$(localedir)\" \
    -DLOG4G_MODULEDIR=\"$(moduledir)\" \
    -I$(top_srcdir) \
    $(GLIB_CFLAGS) \
    $(GMODULE_CFLAGS) \
    $(GOBJECT_CFLAGS) \
    $(LIBXML2_CFLAGS)
liblog4g_@log4g_series_major@_@log4g_series_minor@_la_LDFLAGS = \
    $(GLIB_LIBS) \
    $(GMODULE_LIBS) \
    $(GOBJECT_LIBS) \
    $(LIBXML2_LIBS)
# install development headers
pkgincludedir = $(includedir)/log4g-$(log4g_series)/log4g
pkginclude_HEADERS = \
    appender.h \
    basic-configurator.h \
    disable.h \
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
    ndc.h \
    provision-node.h \
    root-logger.h
helperincludedir = $(includedir)/log4g-$(log4g_series)/log4g/helpers
helperinclude_HEADERS = \
    helpers/appender-attachable-impl.h \
    helpers/default-logger-factory.h \
    helpers/default-module-loader.h \
    helpers/default-repository-selector.h \
    helpers/only-once-error-handler.h \
    helpers/thread.h
interfaceincludedir = $(includedir)/log4g-$(log4g_series)/log4g/interface
interfaceinclude_HEADERS = \
    interface/appender-attachable.h \
    interface/configurator.h \
    interface/error-handler.h \
    interface/logger-factory.h \
    interface/logger-repository.h \
    interface/module-loader.h \
    interface/repository-selector.h
EXTRA_DIST = \
    support/marshal.list
MAINTAINERCLEANFILES = \
    $(srcdir)/Makefile.in

#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# generate marshalling routines
support/marshal.h: support/marshal.list
	glib-genmarshal --header $< > $@
support/marshal.c: support/marshal.list
	glib-genmarshal --body $< > $@
# include directories
INCLUDES = \
    -I$(top_srcdir) \
    -I$(top_srcdir)/src \
    $(GLIB_CFLAGS) \
    $(GMODULE_CFLAGS) \
    $(GOBJECT_CFLAGS) \
    $(LIBXML2_CFLAGS)
# compiler switches
AM_CPPFLAGS = -std=gnu99 -Wall -Werror \
    -DLOG4G_LOCALEDIR=\"$(localedir)\" \
    -DLOG4G_MODULEDIR=\"$(moduledir)\"
# library definition
lib_LTLIBRARIES = liblog4g.la
liblog4g_la_SOURCES = \
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
    log4g/helpers/log.h \
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
liblog4g_la_LDFLAGS = \
    $(GLIB_LIBS) \
    $(GMODULE_LIBS) \
    $(GOBJECT_LIBS) \
    $(LIBXML2_LIBS)
# install development headers
pkgincludedir = $(includedir)/log4g
pkginclude_HEADERS = \
    log4g/appender.h \
    log4g/basic-configurator.h \
    log4g/disable.h \
    log4g/dom-configurator.h \
    log4g/error.h \
    log4g/filter.h \
    log4g/hierarchy.h \
    log4g/layout.h \
    log4g/level.h \
    log4g/log4g.h \
    log4g/log-manager.h \
    log4g/logger.h \
    log4g/logging-event.h \
    log4g/mdc.h \
    log4g/ndc.h \
    log4g/provision-node.h \
    log4g/root-logger.h
helperincludedir = $(includedir)/log4g/helpers
helperinclude_HEADERS = \
    log4g/helpers/appender-attachable-impl.h \
    log4g/helpers/default-logger-factory.h \
    log4g/helpers/default-module-loader.h \
    log4g/helpers/default-repository-selector.h \
    log4g/helpers/only-once-error-handler.h \
    log4g/helpers/thread.h
interfaceincludedir = $(includedir)/log4g/interface
interfaceinclude_HEADERS = \
    log4g/interface/appender-attachable.h \
    log4g/interface/configurator.h \
    log4g/interface/error-handler.h \
    log4g/interface/logger-factory.h \
    log4g/interface/logger-repository.h \
    log4g/interface/module-loader.h \
    log4g/interface/repository-selector.h
EXTRA_DIST = \
    support/marshal.list
MAINTAINERCLEANFILES = \
    $(srcdir)/Makefile.in

#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# generate marshalling routines
%.h: %.list
	glib-genmarshal --header $< > $@
%.c: %.list
	glib-genmarshal --body $< > $@
support/marshal.h: support/marshal.list
support/marshal.c: support/marshal.list
# include directories
INCLUDES = \
    -I$(top_srcdir) \
    -I$(top_srcdir)/src \
    $(GMODULE_CFLAGS) \
    $(GOBJECT_CFLAGS) \
    $(LIBXML2_CFLAGS)
# compiler switches
AM_CPPFLAGS = -std=gnu99 -Wall -Werror
# library definition
lib_LTLIBRARIES = liblog4g.la
liblog4g_la_SOURCES = \
    appender.c \
    appender-attachable.c \
    appender-attachable-impl.c \
    appender-skeleton.c \
    async-appender.c \
    basic-configurator.c \
    configurator.c \
    console-appender.c \
    counting-quiet-writer.c \
    date-layout.c \
    default-logger-factory.c \
    default-repository-selector.c \
    deny-all-filter.c \
    dom-configurator.c \
    error.c \
    error-handler.c \
    file-appender.c \
    filter.c \
    hierarchy.c \
    html-layout.c \
    layout.c \
    level.c \
    level-match-filter.c \
    level-range-filter.c \
    log-manager.c \
    log.c \
    log4g.c \
    logger.c \
    logger-factory.c \
    logger-repository.c \
    logging-event.c \
    mdc.c \
    ndc.c \
    null-appender.c \
    only-once-error-handler.c \
    option-handler.c \
    pattern-converter.c \
    pattern-layout.c \
    pattern-parser.c \
    provision-node.c \
    quiet-writer.c \
    repository-selector.c \
    rolling-file-appender.c \
    root-logger.c \
    simple-layout.c \
    string-match-filter.c \
    support/marshal.c \
    support/marshal.h \
    syslog-appender.c \
    thread.c \
    ttcc-layout.c \
    writer-appender.c \
    xml-layout.c
liblog4g_la_LDFLAGS = \
    $(GMODULE_LIBS) \
    $(GOBJECT_LIBS) \
    $(LIBXML2_LIBS)
# install development headers
pkgincludedir = $(includedir)/log4g
pkginclude_HEADERS = \
    log4g/basic-configurator.h \
    log4g/disable.h \
    log4g/dom-configurator.h \
    log4g/error.h \
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
appenderincludedir = $(includedir)/log4g/appender
appenderinclude_HEADERS = \
    log4g/appender/appender-skeleton.h \
    log4g/appender/async-appender.h \
    log4g/appender/console-appender.h \
    log4g/appender/file-appender.h \
    log4g/appender/null-appender.h \
    log4g/appender/rolling-file-appender.h \
    log4g/appender/syslog-appender.h \
    log4g/appender/writer-appender.h
filterincludedir = $(includedir)/log4g/filter
filterinclude_HEADERS = \
    log4g/filter/deny-all-filter.h \
    log4g/filter/level-match-filter.h \
    log4g/filter/level-range-filter.h \
    log4g/filter/string-match-filter.h \
    log4g/filter.h
helperincludedir = $(includedir)/log4g/helpers
helperinclude_HEADERS = \
    log4g/helpers/appender-attachable-impl.h \
    log4g/helpers/counting-quiet-writer.h \
    log4g/helpers/date-layout.h \
    log4g/helpers/default-logger-factory.h \
    log4g/helpers/default-repository-selector.h \
    log4g/helpers/log.h \
    log4g/helpers/only-once-error-handler.h \
    log4g/helpers/pattern-converter.h \
    log4g/helpers/pattern-parser.h \
    log4g/helpers/quiet-writer.h \
    log4g/helpers/thread.h
interfaceincludedir = $(includedir)/log4g/interface
interfaceinclude_HEADERS = \
    log4g/interface/appender.h \
    log4g/interface/appender-attachable.h \
    log4g/interface/configurator.h \
    log4g/interface/error-handler.h \
    log4g/interface/logger-factory.h \
    log4g/interface/logger-repository.h \
    log4g/interface/option-handler.h \
    log4g/interface/repository-selector.h
layoutincludedir = $(includedir)/log4g/layout
layoutinclude_HEADERS = \
    log4g/layout/html-layout.h \
    log4g/layout/pattern-layout.h \
    log4g/layout/simple-layout.h \
    log4g/layout/ttcc-layout.h \
    log4g/layout/xml-layout.h
EXTRA_DIST = \
    support/glib-2.23/ginterface.h \
    support/marshal.list
MAINTAINERCLEANFILES = Makefile.in

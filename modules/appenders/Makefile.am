#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

module_LTLIBRARIES = \
    liblog4g-appenders.la
liblog4g_appenders_la_SOURCES = \
    async-appender.c \
    async-appender.h \
    console-appender.c \
    console-appender.h \
    counting-quiet-writer.c \
    counting-quiet-writer.h \
    file-appender.c \
    file-appender.h \
    module.c \
    null-appender.c \
    null-appender.h \
    quiet-writer.c \
    quiet-writer.h \
    rolling-file-appender.c \
    rolling-file-appender.h \
    syslog-appender.c \
    syslog-appender.h \
    writer-appender.c \
    writer-appender.h
liblog4g_appenders_la_CFLAGS = \
    -std=gnu99 -Wall -Werror \
    -I$(top_srcdir) \
    -I$(top_srcdir)/src \
    -I$(srcdir) \
    $(GLIB_CFLAGS) \
    $(GOBJECT_CFLAGS)
liblog4g_appenders_la_LDFLAGS = \
    -avoid-version -shared \
    $(GLIB_LIBS) \
    $(GOBJECT_LIBS)
MAINTAINERCLEANFILES = \
    $(srcdir)/Makefile.in

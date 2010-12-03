#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

module_LTLIBRARIES = \
    liblog4g-appenders.la
liblog4g_appenders_la_SOURCES = \
    appender/async-appender.h \
    appender/console-appender.h \
    appender/file-appender.h \
    appender/null-appender.h \
    appender/rolling-file-appender.h \
    appender/syslog-appender.h \
    appender/writer-appender.h \
    async-appender.c \
    console-appender.c \
    counting-quiet-writer.c \
    file-appender.c \
    helpers/counting-quiet-writer.h \
    helpers/quiet-writer.h \
    module.c \
    null-appender.c \
    quiet-writer.c \
    rolling-file-appender.c \
    syslog-appender.c \
    writer-appender.c
liblog4g_appenders_la_CFLAGS = \
    -I$(srcdir) -I$(top_srcdir) \
    $(GLIB_CFLAGS) \
    $(GOBJECT_CFLAGS)
liblog4g_appenders_la_LDFLAGS = \
    -avoid-version -shared \
    $(top_builddir)/log4g/liblog4g-$(log4g_series).la \
    $(GLIB_LIBS) \
    $(GOBJECT_LIBS)

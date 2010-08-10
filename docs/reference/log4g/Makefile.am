#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# We require automake 1.11 at least.
AUTOMAKE_OPTIONS = 1.11

# The name of the module, e.g. 'glib'.
DOC_MODULE = $(PACKAGE_TARNAME)

# Specify the version of the module, e.g. '2'.
DOC_MODULE_VERSION = 1

# The top-level SGML file. You can change this if you want to.
DOC_MAIN_SGML_FILE = $(DOC_MODULE)-docs.sgml

# The directory containing the source code. Relative to $(srcdir).
DOC_SOURCE_DIR = ../../..

# Extra options to pass to gtkdoc-scangobj. Not normally needed.
SCANGOBJ_OPTIONS =

# Extra options to supply to gtkdoc-scan.
SCAN_OPTIONS =

# Extra options to supply to gtkdoc-mkdb.
MKDB_OPTIONS = --sgml-mode --output-format=xml --name-space=log4g

# Extra options to supply to gtkdoc-mktmpl
MKTMPL_OPTIONS =

# Extra options to supply to gtkdoc-mkhtml
MKHTML_OPTIONS =

# Extra options to supply to gtkdoc-fixref. Not normally needed.
FIXXREF_OPTIONS =

# Used for dependencies. The docs will be rebuilt if any of these change.
HFILE_GLOB = \
    $(top_srcdir)/log4g/*.h \
    $(top_srcdir)/log4g/helpers/*.h \
    $(top_srcdir)/log4g/interface/*.h \
    $(top_srcdir)/modules/appenders/appender/*.h \
    $(top_srcdir)/modules/appenders/helpers/*.h \
    $(top_srcdir)/modules/couchdb/appender/*.h \
    $(top_srcdir)/modules/couchdb/layout/*.h \
    $(top_srcdir)/modules/filters/filter/*.h \
    $(top_srcdir)/modules/layouts/helpers/*.h \
    $(top_srcdir)/modules/layouts/layout/*.h

CFILE_GLOB = \
    $(top_srcdir)/log4g/*.c \
    $(top_srcdir)/modules/appenders/*.c \
    $(top_srcdir)/modules/couchdb/*.c \
    $(top_srcdir)/modules/filters/*.c \
    $(top_srcdir)/modules/layouts/*.c

# Extra header to include when scanning, which are not under DOC_SOURCE_DIR
EXTRA_HFILES =

# Header files to ignore when scanning. Use base file name, no paths
IGNORE_HFILES = \
    config.h \
    tests \
    support

# Images to copy into HTML directory.
HTML_IMAGES =

# Extra SGML files that are included by $(DOC_MAIN_SGML_FILE).
content_files = \
    introduction.sgml

# SGML files where gtk-doc abbrevations (#GtkWidget) are expanded
# These files must be listed here *and* in content_files
expand_content_files =

# CFLAGS and LDFLAGS for compiling gtkdoc-scangobj with your library.
# Only needed if you are using gtkdoc-scangobj to dynamically query widget
# signals and properties.
GTKDOC_CFLAGS = \
    -DLOG4G_LOCALEDIR=\"$(localedir)\" \
    -DLOG4G_MODULEDIR=\"$(moduledir)\" \
    -I$(top_srcdir) \
    $(GLIB_CFLAGS) \
    $(GOBJECT_CFLAGS) \
    $(LIBXML2_CFLAGS)

GTKDOC_LIBS = \
    $(top_builddir)/log4g/liblog4g-@log4g_series_major@.@log4g_series_minor@.la

# This includes the standard gtk-doc make rules, copied by gtkdocize.
include $(top_srcdir)/gtk-doc.make

# Other files to distribute
EXTRA_DIST +=

MAINTAINERCLEANFILES = \
    $(srcdir)/$(DOC_MODULE).args $(srcdir)/$(DOC_MODULE).hierarchy \
    $(srcdir)/$(DOC_MODULE).interfaces $(srcdir)/$(DOC_MODULE)-overrides.txt \
    $(srcdir)/$(DOC_MODULE).prerequisites $(srcdir)/$(DOC_MODULE).signals \
    $(srcdir)/$(DOC_MODULE).types Makefile.in $(DOC_MODULE)-scan.c \
    Makefile.in *.bak

-include $(top_srcdir)/git.mk

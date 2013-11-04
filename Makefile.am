#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

SUBDIRS = . docs
ACLOCAL_AMFLAGS = -I m4

# generate marshalling routines
$(top_srcdir)/log4g/marshal.h: log4g/marshal.list
	$(AM_V_GEN)$(GLIB_GENMARSHAL) --prefix=log4g_marshal --header $< > $@

$(top_srcdir)/log4g/marshal.c: log4g/marshal.list
	$(AM_V_GEN)$(GLIB_GENMARSHAL) --prefix=log4g_marshal --body $< > $@

# library definition
lib_LTLIBRARIES = log4g/liblog4g-@series_major@.@series_minor@.la
log4g_liblog4g_@series_major@_@series_minor@_la_SOURCES = \
	log4g/appender.c \
	log4g/appender-attachable.c \
	log4g/appender-attachable-impl.c \
	log4g/basic-configurator.c \
	log4g/configurator.c \
	log4g/default-logger-factory.c \
	log4g/default-module-loader.c \
	log4g/default-repository-selector.c \
	log4g/dom-configurator.c \
	log4g/error.c \
	log4g/error-handler.c \
	log4g/filter.c \
	log4g/hierarchy.c \
	log4g/layout.c \
	log4g/level.c \
	log4g/log-manager.c \
	log4g/log.c \
	log4g/helpers/log.h \
	log4g/log4g.c \
	log4g/logger.c \
	log4g/logger-factory.c \
	log4g/logger-repository.c \
	log4g/logging-event.c \
	log4g/mdc.c \
	log4g/module.c \
	log4g/module-loader.c \
	log4g/ndc.c \
	log4g/only-once-error-handler.c \
	log4g/provision-node.c \
	log4g/repository-selector.c \
	log4g/root-logger.c \
	log4g/marshal.c \
	log4g/marshal.h \
	log4g/thread.c

BUILT_SOURCES = \
	log4g/marshal.c \
	log4g/marshal.h

# include directories & compiler switches
log4g_liblog4g_@series_major@_@series_minor@_la_CFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GMODULE_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(LIBXML_CFLAGS) \
	-DLOG4G_LOCALEDIR=\"$(localedir)\" \
	-DLOG4G_MODULEDIR=\"$(moduledir)\"

log4g_liblog4g_@series_major@_@series_minor@_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	$(GLIB_LIBS) \
	$(GMODULE_LIBS) \
	$(GOBJECT_LIBS) \
	$(LIBXML_LIBS)

# install development headers
pkgincludedir = $(includedir)/log4g-$(series)/log4g
pkginclude_HEADERS = \
	log4g/appender.h \
	log4g/basic-configurator.h \
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
	log4g/module.h \
	log4g/ndc.h \
	log4g/provision-node.h \
	log4g/root-logger.h

disableincludedir = $(pkgincludedir)
disableinclude_HEADERS = \
	log4g/disable.h

helperincludedir = $(includedir)/log4g-$(series)/log4g/helpers
helperinclude_HEADERS = \
	log4g/helpers/appender-attachable-impl.h \
	log4g/helpers/default-logger-factory.h \
	log4g/helpers/default-module-loader.h \
	log4g/helpers/default-repository-selector.h \
	log4g/helpers/only-once-error-handler.h \
	log4g/helpers/thread.h

interfaceincludedir = $(includedir)/log4g-$(series)/log4g/interface
interfaceinclude_HEADERS = \
	log4g/interface/appender-attachable.h \
	log4g/interface/configurator.h \
	log4g/interface/error-handler.h \
	log4g/interface/logger-factory.h \
	log4g/interface/logger-repository.h \
	log4g/interface/module-loader.h \
	log4g/interface/repository-selector.h

# configure tests
check_PROGRAMS =

check_PROGRAMS += tests/appender-attachable-impl-test
tests_appender_attachable_impl_test_SOURCES = tests/appender-attachable-impl-test.c
tests_appender_attachable_impl_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_appender_attachable_impl_test_LDFLAGS = $(GLIB_LIBS)
tests_appender_attachable_impl_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/basic-configurator-test
tests_basic_configurator_test_SOURCES = tests/basic-configurator-test.c
tests_basic_configurator_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_basic_configurator_test_LDFLAGS = $(GLIB_LIBS)
tests_basic_configurator_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/dom-configurator-test
tests_dom_configurator_test_SOURCES = tests/dom-configurator-test.c
tests_dom_configurator_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_dom_configurator_test_LDFLAGS = $(GLIB_LIBS)
tests_dom_configurator_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/hierarchy-test
tests_hierarchy_test_SOURCES = tests/hierarchy-test.c
tests_hierarchy_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_hierarchy_test_LDFLAGS = $(GLIB_LIBS)
tests_hierarchy_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/level-test
tests_level_test_SOURCES = tests/level-test.c
tests_level_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_level_test_LDFLAGS = $(GLIB_LIBS)
tests_level_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/log4g-test
tests_log4g_test_SOURCES = tests/log4g-test.c
tests_log4g_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_log4g_test_LDFLAGS = $(GLIB_LIBS)
tests_log4g_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/logger-test
tests_logger_test_SOURCES = tests/logger-test.c
tests_logger_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_logger_test_LDFLAGS = $(GLIB_LIBS)
tests_logger_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/mdc-test
tests_mdc_test_SOURCES = tests/mdc-test.c
tests_mdc_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_mdc_test_LDFLAGS = $(GLIB_LIBS)
tests_mdc_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/module-test
tests_module_test_SOURCES = tests/module-test.c
tests_module_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_module_test_LDFLAGS = $(GLIB_LIBS)
tests_module_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/ndc-test
tests_ndc_test_SOURCES = tests/ndc-test.c
tests_ndc_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_ndc_test_LDFLAGS = $(GLIB_LIBS)
tests_ndc_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/only-once-error-handler-test
tests_only_once_error_handler_test_SOURCES = tests/only-once-error-handler-test.c
tests_only_once_error_handler_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_only_once_error_handler_test_LDFLAGS = $(GLIB_LIBS)
tests_only_once_error_handler_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/provision-node-test
tests_provision_node_test_SOURCES = tests/provision-node-test.c
tests_provision_node_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_provision_node_test_LDFLAGS = $(GLIB_LIBS)
tests_provision_node_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

# Module definitions
module_LTLIBRARIES =

if LOG4G_WITH_APPENDERS
module_LTLIBRARIES += modules/appenders/liblog4g-appenders.la

modules_appenders_liblog4g_appenders_la_SOURCES = \
	modules/appenders/appender/async-appender.h \
	modules/appenders/appender/console-appender.h \
	modules/appenders/appender/file-appender.h \
	modules/appenders/appender/null-appender.h \
	modules/appenders/appender/rolling-file-appender.h \
	modules/appenders/appender/syslog-appender.h \
	modules/appenders/appender/writer-appender.h \
	modules/appenders/async-appender.c \
	modules/appenders/console-appender.c \
	modules/appenders/counting-quiet-writer.c \
	modules/appenders/file-appender.c \
	modules/appenders/helpers/counting-quiet-writer.h \
	modules/appenders/helpers/quiet-writer.h \
	modules/appenders/module.c \
	modules/appenders/null-appender.c \
	modules/appenders/quiet-writer.c \
	modules/appenders/rolling-file-appender.c \
	modules/appenders/syslog-appender.c \
	modules/appenders/writer-appender.c

modules_appenders_liblog4g_appenders_la_CFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/modules/appenders \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS)

modules_appenders_liblog4g_appenders_la_LDFLAGS = \
	-avoid-version \
	-shared \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS)

modules_appenders_liblog4g_appenders_la_LIBADD = \
	$(top_builddir)/log4g/liblog4g-$(series).la

# configure tests
check_PROGRAMS += tests/async-appender-test
tests_async_appender_test_SOURCES = tests/async-appender-test.c
tests_async_appender_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_async_appender_test_LDFLAGS = $(GLIB_LIBS)
tests_async_appender_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/console-appender-test
tests_console_appender_test_SOURCES = tests/console-appender-test.c
tests_console_appender_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_console_appender_test_LDFLAGS = $(GLIB_LIBS)
tests_console_appender_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/file-appender-test
tests_file_appender_test_SOURCES = tests/file-appender-test.c
tests_file_appender_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_file_appender_test_LDFLAGS = $(GLIB_LIBS)
tests_file_appender_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/syslog-appender-test
tests_syslog_appender_test_SOURCES = tests/syslog-appender-test.c
tests_syslog_appender_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_syslog_appender_test_LDFLAGS = $(GLIB_LIBS)
tests_syslog_appender_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/writer-appender-test
tests_writer_appender_test_SOURCES = tests/writer-appender-test.c
tests_writer_appender_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_writer_appender_test_LDFLAGS = $(GLIB_LIBS)
tests_writer_appender_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/rolling-file-appender-test
tests_rolling_file_appender_test_SOURCES = tests/rolling-file-appender-test.c
tests_rolling_file_appender_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_rolling_file_appender_test_LDFLAGS = $(GLIB_LIBS)
tests_rolling_file_appender_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la
endif

if LOG4G_WITH_FILTERS
module_LTLIBRARIES += modules/filters/liblog4g-filters.la

modules_filters_liblog4g_filters_la_SOURCES = \
	modules/filters/deny-all-filter.c \
	modules/filters/filter/deny-all-filter.h \
	modules/filters/filter/level-range-filter.h \
	modules/filters/filter/level-match-filter.h \
	modules/filters/filter/string-match-filter.h \
	modules/filters/level-range-filter.c \
	modules/filters/level-match-filter.c \
	modules/filters/module.c \
	modules/filters/string-match-filter.c

modules_filters_liblog4g_filters_la_CFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/modules/filters \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS)

modules_filters_liblog4g_filters_la_LDFLAGS = \
	-avoid-version \
	-shared \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS)

modules_filters_liblog4g_filters_la_LIBADD = \
	$(top_builddir)/log4g/liblog4g-$(series).la

# configure tests
check_PROGRAMS += tests/deny-all-filter-test
tests_deny_all_filter_test_SOURCES = tests/deny-all-filter-test.c
tests_deny_all_filter_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_deny_all_filter_test_LDFLAGS = $(GLIB_LIBS)
tests_deny_all_filter_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/level-match-filter-test
tests_level_match_filter_test_SOURCES = tests/level-match-filter-test.c
tests_level_match_filter_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_level_match_filter_test_LDFLAGS = $(GLIB_LIBS)
tests_level_match_filter_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/level-range-filter-test
tests_level_range_filter_test_SOURCES = tests/level-range-filter-test.c
tests_level_range_filter_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_level_range_filter_test_LDFLAGS = $(GLIB_LIBS)
tests_level_range_filter_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/string-match-filter-test
tests_string_match_filter_test_SOURCES = tests/string-match-filter-test.c
tests_string_match_filter_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_string_match_filter_test_LDFLAGS = $(GLIB_LIBS)
tests_string_match_filter_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la
endif

if LOG4G_WITH_LAYOUTS
module_LTLIBRARIES += modules/layouts/liblog4g-layouts.la

modules_layouts_liblog4g_layouts_la_SOURCES = \
	modules/layouts/date-layout.c \
	modules/layouts/html-layout.c \
	modules/layouts/layout/date-layout.h \
	modules/layouts/layout/html-layout.h \
	modules/layouts/layout/simple-layout.h \
	modules/layouts/layout/ttcc-layout.h \
	modules/layouts/layout/xml-layout.h \
	modules/layouts/module.c \
	modules/layouts/pattern-converter.c \
	modules/layouts/pattern-layout.c \
	modules/layouts/helpers/pattern-converter.h \
	modules/layouts/helpers/pattern-parser.h \
	modules/layouts/layout/pattern-layout.h \
	modules/layouts/pattern-parser.c \
	modules/layouts/simple-layout.c \
	modules/layouts/ttcc-layout.c \
	modules/layouts/xml-layout.c

modules_layouts_liblog4g_layouts_la_CFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/modules/layouts \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS)

modules_layouts_liblog4g_layouts_la_LDFLAGS = \
	-avoid-version \
	-shared \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS)

modules_layouts_liblog4g_layouts_la_LIBADD = \
	$(top_builddir)/log4g/liblog4g-$(series).la

# configure tests
check_PROGRAMS += tests/html-layout-test
tests_html_layout_test_SOURCES = tests/html-layout-test.c
tests_html_layout_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_html_layout_test_LDFLAGS = $(GLIB_LIBS)
tests_html_layout_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/pattern-layout-test
tests_pattern_layout_test_SOURCES = tests/pattern-layout-test.c
tests_pattern_layout_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_pattern_layout_test_LDFLAGS = $(GLIB_LIBS)
tests_pattern_layout_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/simple-layout-test
tests_simple_layout_test_SOURCES = tests/simple-layout-test.c
tests_simple_layout_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_simple_layout_test_LDFLAGS = $(GLIB_LIBS)
tests_simple_layout_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/ttcc-layout-test
tests_ttcc_layout_test_SOURCES = tests/ttcc-layout-test.c
tests_ttcc_layout_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_ttcc_layout_test_LDFLAGS = $(GLIB_LIBS)
tests_ttcc_layout_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la

check_PROGRAMS += tests/xml-layout-test
tests_xml_layout_test_SOURCES = tests/xml-layout-test.c
tests_xml_layout_test_CFLAGS = -I$(top_srcdir) $(GLIB_CFLAGS)
tests_xml_layout_test_LDFLAGS = $(GLIB_LIBS)
tests_xml_layout_test_LDADD = $(top_builddir)/log4g/liblog4g-$(series).la
endif

# configure test environment
LOG4G_MODULE_PATH = $(top_builddir)/modules/appenders/.libs:$(top_builddir)/modules/filters/.libs:$(top_builddir)/modules/layouts/.libs

TESTS_ENVIRONMENT = \
	LOG4G_MODULE_SYSTEM_PATH="" \
	LOG4G_MODULE_PATH=$(LOG4G_MODULE_PATH)

TESTS = $(check_PROGRAMS)

# generate XML report
tests/log4g.xml: $(check_PROGRAMS)
	$(AM_V_GEN)srcdir=@srcdir@ \
	LOG4G_MODULE_SYSTEM_PATH="" \
	LOG4G_MODULE_PATH=$(LOG4G_MODULE_PATH) \
		gtester -k -o $@ $(check_PROGRAMS)

# generate HTML report
tests/log4g.html: tests/log4g.xml
	$(AM_V_GEN)gtester-report $< > $@

# install pkg-config script
log4g-$(series).pc: log4g.pc
	$(AM_V_GEN)cp $< $@

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = log4g-$(series).pc

# install DTD
dtddir = $(datadir)/log4g/dtd/$(series)
dtd_DATA = log4g/log4g.dtd

CLEANFILES = \
	tests/log4g.xml \
	tests/dom-configurator-*.txt \
	tests/*-test.txt \
	tests/*-test.txt.* \
	tests/*-test.html

DISTCLEANFILES = \
	log4g-$(series).pc \
	tests/log4g.html \
	core core.*

EXTRA_DIST = \
	README.md \
	log4g/log4g.dtd \
	log4g/marshal.list \
	tests/dom-configurator-001.xml \
	tests/perf-001.xml

DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc

#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# include directories
INCLUDES = \
    -I$(top_srcdir)/src \
    $(GLIB_CFLAGS) \
    $(GTHREAD_CFLAGS)
# add libraries
LIBS = \
    $(top_builddir)/src/liblog4g.la \
    $(GLIB_LIBS) $(GTHREAD_LIBS)
# compiler switches
AM_CPPFLAGS = -std=gnu99 -Wall -Werror
# add unit tests
check_PROGRAMS = \
    appender-attachable-impl-test \
    async-appender-test \
    basic-configurator-test \
    console-appender-test \
    deny-all-filter-test \
    dom-configurator-test \
    file-appender-test \
    hierarchy-test \
    level-test \
    log4g-test \
    logger-test \
    mdc-test \
    ndc-test \
    only-once-error-handler-test \
    pattern-layout-test \
    provision-node-test \
    quiet-writer-test \
    rolling-file-appender-test \
    writer-appender-test
# add source files
appender_attachable_impl_test_SOURCES = appender-attachable-impl-test.c
async_appender_test_SOURCES = async-appender-test.c
basic_configurator_test_SOURCES = basic-configurator-test.c
console_appender_test_SOURCES = console-appender-test.c
deny_all_filter_test_SOURCES = deny-all-filter-test.c
dom_configurator_test_SOURCES = dom-configurator-test.c
file_appender_test_SOURCES = file-appender-test.c
hierarchy_test_SOURCES = hierarchy-test.c
level_test_SOURCES = level-test.c
log4g_test_SOURCES = log4g-test.c
logger_test_SOURCES = logger-test.c
mdc_test_SOURCES = mdc-test.c
ndc_test_SOURCES = ndc-test.c
only_once_error_handler_test_SOURCES = only-once-error-handler-test.c
pattern_layout_test_SOURCES = pattern-layout-test.c
provision_node_test_SOURCES = provision-node-test.c
quiet_writer_test_SOURCES = quiet-writer-test.c
rolling_file_appender_test_SOURCES = rolling-file-appender-test.c
writer_appender_test_SOURCES = writer-appender-test.c
# run tests
check-local: $(check_PROGRAMS)
	@LOG4G_DOM_CONFIGURATOR_TEST=@top_srcdir@/tests gtester $(check_PROGRAMS)
# generate XML report
log4g.xml: $(check_PROGRAMS)
	gtester -k -o $@ $(check_PROGRAMS)
# generate HTML report
log4g.html: log4g.xml
	gtester-report $< > $@
# generate report
report: log4g.html
EXTRA_DIST = \
    dom-configurator-001.xml
.PHONY: report
DISTCLEANFILES = \
    *-test.txt *-test.txt.* *-test.html log4g.html log4g.xml \
    dom-configurator-*.txt
MAINTAINERCLEANFILES = Makefile.in

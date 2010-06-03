#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# include directories
INCLUDES = \
    -I$(top_srcdir) \
    $(GLIB_CFLAGS) \
    $(GTHREAD_CFLAGS)
# add libraries
LIBS = \
    $(top_builddir)/log4g/liblog4g-$(log4g_series).la \
    $(GLIB_LIBS) $(GTHREAD_LIBS)
# add unit tests
check_PROGRAMS = \
    appender-attachable-impl-test \
    async-appender-test \
    basic-configurator-test \
    console-appender-test \
    couchdb-appender-test \
    couchdb-layout-test \
    deny-all-filter-test \
    dom-configurator-test \
    file-appender-test \
    hierarchy-test \
    html-layout-test \
    level-match-filter-test \
    level-range-filter-test \
    level-test \
    log4g-test \
    logger-test \
    mdc-test \
    module-test \
    ndc-test \
    only-once-error-handler-test \
    pattern-layout-test \
    provision-node-test \
    rolling-file-appender-test \
    simple-layout-test \
    string-match-filter-test \
    syslog-appender-test \
    ttcc-layout-test \
    writer-appender-test \
    xml-layout-test
# add source files
appender_attachable_impl_test_SOURCES = appender-attachable-impl-test.c
async_appender_test_SOURCES = async-appender-test.c
basic_configurator_test_SOURCES = basic-configurator-test.c
console_appender_test_SOURCES = console-appender-test.c
couchdb_layout_test_SOURCES = couchdb-layout-test.c
couchdb_appender_test_SOURCES = couchdb-appender-test.c
deny_all_filter_test_SOURCES = deny-all-filter-test.c
dom_configurator_test_SOURCES = dom-configurator-test.c
file_appender_test_SOURCES = file-appender-test.c
hierarchy_test_SOURCES = hierarchy-test.c
html_layout_test_SOURCES = html-layout-test.c
level_match_filter_test_SOURCES = level-match-filter-test.c
level_range_filter_test_SOURCES = level-range-filter-test.c
level_test_SOURCES = level-test.c
log4g_test_SOURCES = log4g-test.c
logger_test_SOURCES = logger-test.c
mdc_test_SOURCES = mdc-test.c
module_test_SOURCES = module-test.c
ndc_test_SOURCES = ndc-test.c
only_once_error_handler_test_SOURCES = only-once-error-handler-test.c
pattern_layout_test_SOURCES = pattern-layout-test.c
provision_node_test_SOURCES = provision-node-test.c
rolling_file_appender_test_SOURCES = rolling-file-appender-test.c
simple_layout_test_SOURCES = simple-layout-test.c
string_match_filter_test_SOURCES = string-match-filter-test.c
syslog_appender_test_SOURCES = syslog-appender-test.c
ttcc_layout_test_SOURCES = ttcc-layout-test.c
writer_appender_test_SOURCES = writer-appender-test.c
xml_layout_test_SOURCES = xml-layout-test.c
# generate XML report
log4g.xml: $(check_PROGRAMS)
	@srcdir=@srcdir@ \
        LOG4G_MODULE_SYSTEM_PATH="" \
        LOG4G_MODULE_PATH="../modules/appenders/.libs:../modules/couchdb/.libs:../modules/filters/.libs:../modules/layouts/.libs" \
	    gtester -k -o $@ $(check_PROGRAMS)
# generate HTML report
log4g.html: log4g.xml
	gtester-report $< > $@
# generate report
check-local: log4g.html
EXTRA_DIST = \
    dom-configurator-001.xml \
    perf-001.xml
CLEANFILES = \
    log4g.xml dom-configurator-*.txt *-test.txt *-test.txt.* *-test.html
DISTCLEANFILES = \
    log4g.html core core.*
MAINTAINERCLEANFILES = \
    $(srcdir)/Makefile.in

#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

SUBDIRS = src modules tests doc
EXTRA_DIST = resources
ACLOCAL_AMFLAGS = -I m4
# install pkg-config script
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = resources/log4g.pc
# install DTD
dtddir = $(datadir)/log4g/dtd/1.0
dtd_DATA = resources/dtd/1.0/log4g.dtd
# add `doc' target
doc:
	cd doc && $(MAKE) $(MAKEFLAGS) doc
# add `report' target
report:
	cd tests && $(MAKE) $(MAKEFLAGS) report
.PHONY: doc report
# cleanup
DISTCLEANFILES = \
    doltcompile doltlibtool
MAINTAINERCLEANFILES = \
    $(top_srcdir)/aclocal.m4 $(top_srcdir)/config.guess \
    $(top_srcdir)/config.h.in $(top_srcdir)/config.h.in~ \
    $(top_srcdir)/config.sub $(top_srcdir)/configure \
    $(top_srcdir)/depcomp $(top_srcdir)/INSTALL \
    $(top_srcdir)/install-sh $(top_srcdir)/ltmain.sh \
    $(top_srcdir)/m4/lt*.m4 $(top_srcdir)/m4/libtool.m4 \
    $(top_srcdir)/Makefile.in $(top_srcdir)/missing

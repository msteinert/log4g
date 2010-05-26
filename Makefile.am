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
    aclocal.m4 config.guess config.sub configure depcomp install-sh \
    ltmain.sh Makefile.in missing config.h.in config.h.in~ m4/lt*.m4 \
    m4/libtool.m4 INSTALL

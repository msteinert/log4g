#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

SUBDIRS = doc log4g modules tests
EXTRA_DIST = resources
ACLOCAL_AMFLAGS = -I m4
DISTCHECK_CONFIGURE_FLAGS = --enable-introspection
# install pkg-config script
resources/log4g-$(log4g_series).pc: resources/log4g.pc
	cp $< $@
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = resources/log4g-$(log4g_series).pc
# install DTD
dtddir = $(datadir)/log4g/dtd/$(log4g_series)
dtd_DATA = resources/log4g.dtd
# add `doc' target
doc:
	cd doc && $(MAKE) $(MAKEFLAGS) doc
# add `report' target
report:
	cd tests && $(MAKE) $(MAKEFLAGS) report
.PHONY: doc report
# cleanup
DISTCLEANFILES = \
    doltcompile doltlibtool resources/log4g-$(log4g_series).pc
MAINTAINERCLEANFILES = \
    $(srcdir)/aclocal.m4 $(srcdir)/config.guess $(srcdir)/config.h.in \
    $(srcdir)/config.h.in~ $(srcdir)/config.sub $(srcdir)/configure \
    $(srcdir)/depcomp $(srcdir)/INSTALL $(srcdir)/install-sh \
    $(srcdir)/ltmain.sh $(srcdir)/m4/lt*.m4 $(srcdir)/m4/libtool.m4 \
    $(srcdir)/Makefile.in $(srcdir)/missing

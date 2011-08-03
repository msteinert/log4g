#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

SUBDIRS = log4g modules tests docs
EXTRA_DIST = resources
ACLOCAL_AMFLAGS = -I m4
DISTCHECK_CONFIGURE_FLAGS = --enable-introspection --enable-gtk-doc
# install pkg-config script
resources/log4g-$(series).pc: resources/log4g.pc
	$(AM_V_GEN)cp $< $@
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = resources/log4g-$(series).pc
# install DTD
dtddir = $(datadir)/log4g/dtd/$(series)
dtd_DATA = resources/log4g.dtd
# cleanup
DISTCLEANFILES = \
	resources/log4g-$(series).pc

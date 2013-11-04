#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

SUBDIRS = log4g modules tests docs
ACLOCAL_AMFLAGS = -I m4

# install pkg-config script
resources/log4g-$(series).pc: resources/log4g.pc
	$(AM_V_GEN)cp $< $@
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = resources/log4g-$(series).pc

# install DTD
dtddir = $(datadir)/log4g/dtd/$(series)
dtd_DATA = resources/log4g.dtd

DISTCLEANFILES = \
	resources/log4g-$(series).pc

EXTRA_DIST = resources
DISTCHECK_CONFIGURE_FLAGS = --enable-introspection --enable-gtk-doc

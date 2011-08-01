#!/bin/sh

test -n "$srcdir" || srcdir=`dirname $0`
test -n "$srcdir" || srcdir=.

olddir=`pwd`
cd $srcdir

GTKDOCIZE=`which gtkdocize`
if test -n $GTKDOCIZE; then
	gtkdocize --copy --docdir docs --flavour no-tmpl || exit $?
fi

AUTORECONF=`which autoreconf`
if test -z $AUTORECONF; then
	echo "*** No autoreconf found, please install it ***"
	exit 1
else
	autoreconf --force --install --verbose || exit $?
fi

cd "$olddir"
test -n "$NOCONFIGURE" || "$srcdir/configure" "$@"

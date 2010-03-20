#!/bin/sh

# Revision History
# 8-11-2009 Mike Steinert
#  -initial creation

SUBDIRS=""

echo auto-generating in `pwd`
autoreconf -I m4 --install --force

for dir in $SUBDIRS; do
   cd $dir && ./autogen.sh
done

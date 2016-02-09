#!/bin/bash

rm -rf aclocal.m4 autom4te.cache stamp-h1 libtool configure config.* Makefile.in Makefile config/* m4/*

find -type f -name "*~" -print0 | xargs -0 rm -f
find -type f -name "*.sw[po]" -print0 | xargs -0 rm -f
find -type f -name "Makefile" -print0 | xargs -0 rm -f
find -type f -name "Makefile.in" -print0 | xargs -0 rm -f

find -type d -name .deps -print0 | xargs -0 rm -rf


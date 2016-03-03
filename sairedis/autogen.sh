#!/bin/bash

if [ `uname` == "Darwin" ]; then
    LIBTOOLIZE=glibtoolize
else
    LIBTOOLIZE=libtoolize
fi

$LIBTOOLIZE --force --copy &&
autoreconf --force --install -I m4
rm -Rf autom4te.cache


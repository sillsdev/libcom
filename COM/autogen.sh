#!/bin/sh
set -ex
libtoolize --automake
aclocal
autoheader
automake --add-missing
autoconf

#! /bin/bash
#
#	$Id$
#
#	Win32Base test script
#
#	MarkS - 2007-12-10
#
# Win32 Compatibility Library
# Copyright (C) 2007 SIL International
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#
# http://www.gnu.org/licenses/lgpl.html
#

ARCH=$(uname -m)
BUILD_DIR=build$ARCH

# Location of ICU so files
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(icu-config --libdir)
# Location of libcom
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../COM/$BUILD_DIR/src/.libs
export LD_LIBRARY_PATH

TESTS="TestGUID TestOutputDebugString TestHacks"
for Test in $TESTS
do
	echo Testing $Test
	$BUILD_DIR/$Test && echo Successful test of $Test || { echo Failure testing $Test; exit 1; }
done

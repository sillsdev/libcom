#
#	$Id$
#
#	COM Support Library root Automake file
#
#	MarkS - 2007-01-25
#
# COM Support Library
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

# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 1.4

# SUBDIRS = src test
SUBDIRS = src

EXTRA_DIST = \
	LICENSE \
	lgpl-2.1.txt \
	README \
	include/COM.h \
	include/COMInterfaces.h \
	include/COMLibrary.h \
	include/COMPointers.h \
	include/BasicTypes.h \
	include/WinError.h \
	include/oaidl.h \
	include/ocidl.h \
	include/Ole2.h \
	include/Oleacc.h \
	include/rpc.h \
	include/rpcndr.h \
	include/comdef.h \
	remote-midl/midl \
	remote-midl/tlbimp 

# Manually specify test/ files for inclusion in "make dist" if "test" is not part of SUBDIRS
EXTRA_DIST += \
 test/test.sh \
 test/IBakery.idl \
 test/Jar_GUIDs.cpp \
 test/tools.h \
 test/IBall.idh \
 test/IBall.idl \
 test/Makefile.am \
 test/Makefile.in \
 test/Wall.h \
 test/jar.tli \
 test/ComClient.cs \
 test/raw-midl/README \
 test/raw-midl/IBakery_idl.h \
 test/raw-midl/jar.tli \
 test/raw-midl/IBallMetadata.dll \
 test/raw-midl/jar.tlh \
 test/raw-midl/IBall_iid.h \
 test/raw-midl/IBakeryMetadata.dll \
 test/raw-midl/IBall.tlb \
 test/raw-midl/IBall_idl.h \
 test/Bag.h \
 test/example_components.map \
 test/fix-midl.sh \
 test/Bakery.cpp \
 test/Ball.h \
 test/IWall.idl \
 test/Ball.cpp \
 test/Bag.cpp \
 test/Jar.cs \
 test/jar.tlh \
 test/Tester.cs \
 test/IBagLib.idl \
 test/Wall.cpp \
 test/IBag.idh \
 test/CClient.cc \
 test/extract_iids.sh \
 test/components.map \
 test/Bakery.h \
 test/BakeryJarTestClient.cs

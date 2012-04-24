#
#	$Id$
#
#	libcom test automake file
#
#	MarkS - 2007-01-25
#
# COM Support Library Test
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

Win32BaseInc = $(top_srcdir)/../Win32Base/include
Win32MoreInc = $(top_srcdir)/../Win32More/include
Win32BaseLib = $(top_srcdir)/../Win32Base/src/build$(FWARCH)
Win32MoreLib = $(top_srcdir)/../Win32More/src/build$(FWARCH)

if USE_FW_GENERIC_FACTORY
AM_CPPFLAGS = -DUSE_FW_GENERIC_FACTORY -DUSE_FW_MODULE_ENTRY -DCOM_NO_WINDOWS_H
FieldWorksRootDir = $(FWROOT)
FieldWorksOutputDebugDir = $(FieldWorksRootDir)/Output_$(FWARCH)/Debug
FieldWorksObjDebugDir = $(FieldWorksRootDir)/Obj_$(FWARCH)/Debug
IcuConfig = $(FieldWorksRootDir)/Lib/src/icu/install$(FWARCH)/bin/icu-config
BUILD_ROOT = $(FieldWorksRootDir)
-include $(BUILD_ROOT)/Bld/_user.mak.lnx
else
AM_CPPFLAGS = -DNO_FW
IcuConfig = icu-config
endif

AUTOMAKE_OPTIONS = color-tests

noinst_PROGRAMS = comtest ComClient.exe BakeryJarTestClient.exe Tester.exe
# TODO: ball.so shouldn't be installed, just built.
lib_LTLIBRARIES = ball.la bakery.la wall.la bag.la

INCLUDES = \
	$(all_includes) \
	-I$(top_srcdir)/include \
	-I$(Win32BaseInc) \
	-I$(Win32MoreInc) \
	$$($(IcuConfig) --cppflags)

AM_LDFLAGS = \
	$$($(IcuConfig) --ldflags) \
	$(all_libraries) \
	-luuid

if USE_FW_GENERIC_FACTORY
INCLUDES += \
	-I$(FieldWorksRootDir)/Include \
	-I$(FieldWorksRootDir)/Src/Generic
endif

MIDL = $(top_srcdir)/remote-midl/midl
TLBIMP = $(top_srcdir)/remote-midl/tlbimp
midlIncludeDir = "C:/MicrosoftPlatformSDKforWindowsServer2003R2/Include"

bag_la_SOURCES = \
	Bag.cpp \
	IBag_GUIDs.cpp \
	IBall_GUIDs.cpp

#.libs/ball.o 
bag_la_LIBADD = \
	-L../src/.libs -lcom 
if USE_FW_GENERIC_FACTORY
bag_la_LIBADD += \
	$(FieldWorksOutputDebugDir)/libGeneric.a \
	$(FieldWorksObjDebugDir)/Generic/ModuleEntry.o \
	-L$(Win32BaseLib) -lWin32Base \
	-L$(Win32MoreLib) -lWin32More \
	-lpthread
endif
bag_la_LDFLAGS = -module $(AM_LDFLAGS)


wall_la_SOURCES = \
	Wall.cpp \
	IWall_GUIDs.cpp
wall_la_LIBADD = \
	-L../src/.libs -lcom 
if USE_FW_GENERIC_FACTORY
wall_la_LIBADD += \
	$(FieldWorksOutputDebugDir)/libGeneric.a \
	$(FieldWorksObjDebugDir)/Generic/ModuleEntry.o \
	-L$(Win32BaseLib) -lWin32Base \
	-L$(Win32MoreLib) -lWin32More \
	-lpthread
endif
wall_la_LDFLAGS = -module $(AM_LDFLAGS)

bakery_la_SOURCES = \
	Bakery.cpp \
	IBakery_GUIDs.cpp \
	Jar_GUIDs.cpp
bakery_la_LIBADD = \
	../src/libcom.la 
if USE_FW_GENERIC_FACTORY
bakery_la_LIBADD += \
	$(FieldWorksOutputDebugDir)/libGeneric.a \
	$(FieldWorksObjDebugDir)/Generic/ModuleEntry.o \
	-L$(Win32BaseLib) -lWin32Base \
	-L$(Win32MoreLib) -lWin32More \
	-lpthread
endif
bakery_la_LDFLAGS = -module $(AM_LDFLAGS)


ball_la_SOURCES = \
	Ball.cpp \
	IBall_GUIDs.cpp 

ball_la_LIBADD = \
	../src/libcom.la 
if USE_FW_GENERIC_FACTORY
ball_la_LIBADD += \
	$(FieldWorksOutputDebugDir)/libGeneric.a \
	$(FieldWorksObjDebugDir)/Generic/ModuleEntry.o \
	-L$(Win32BaseLib) -lWin32Base \
	-L$(Win32MoreLib) -lWin32More \
	-lpthread
endif
ball_la_LDFLAGS = -module $(AM_LDFLAGS)

comtest_SOURCES = CClient.cc
comtest_LDADD = IBall_GUIDs.o ../src/libcom.la
comtest_LDFLAGS = -ldl $(AM_LDFLAGS)

EXTRA_DIST = \
	test.sh \
	Jar.cs \
	fix-midl.sh \
	extract_iids.sh \
	Bakery.h \
	Wall.h \
	Bag.h \
	jar.tlh \
	jar.tli \
	Ball.h \
	tools.h \
	raw-midl/IBakery_idl.h \
	raw-midl/IBakeryMetadata.dll \
	raw-midl/IBall_idl.h \
	raw-midl/IBall_iid.h \
	raw-midl/IBallMetadata.dll \
	raw-midl/IBall.tlb \
	raw-midl/jar.tlh \
	raw-midl/jar.tli \
	raw-midl/README \
	IWall.idl \
	IBagLib.idl \
	IBag.idh \
	IBall.idh \
	example_components.map 

CSC = gmcs
CSC_FLAGS = -debug
Tester_exe_SOURCES = Tester.cs
Tester_exe_REFERENCES = IWallInterop.dll IBagInterop.dll
ComClient_exe_SOURCES = ComClient.cs
ComClient_exe_REFERENCES = IBallMetadata.dll
BakeryJarTestClient_exe_SOURCES = BakeryJarTestClient.cs
BakeryJarTestClient_exe_REFERENCES = IBakeryMetadata.dll Jar.dll
Jar_dll_SOURCES_NonAM = Jar.cs
Jar_dll_REFERENCES_NonAM = 
Jar_dll_CSC_FLAGS_NonAM = -target:library

#IWall.idl
Tester.exe: $(Tester_exe_SOURCES) $(Tester_exe_REFERENCES)
	$(CSC) $(CSC_FLAGS) -out:$@ $(^:%.dll=-r:%)
ComClient.exe: $(ComClient_exe_SOURCES) $(ComClient_exe_REFERENCES)
	$(CSC) $(CSC_FLAGS) -out:$@ $(^:%.dll=-r:%)
BakeryJarTestClient.exe: $(BakeryJarTestClient_exe_SOURCES) $(BakeryJarTestClient_exe_REFERENCES)
	$(CSC) $(CSC_FLAGS) -out:$@ $(^:%.dll=-r:%)
Jar.dll: $(Jar_dll_SOURCES_NonAM) $(Jar_dll_REFERENCES_NonAM)
	$(CSC) $(CSC_FLAGS) $(Jar_dll_CSC_FLAGS_NonAM) -out:$@ $(^:%.dll=-r:%)

IBall_idl.h: raw-midl/IBall_idl.h fix-midl.sh
	$(srcdir)/fix-midl.sh $< >$@

IBakery_idl.h: raw-midl/IBakery_idl.h fix-midl.sh
	$(srcdir)/fix-midl.sh $< >$@

IBall_GUIDs.cpp: IBall_idl.h extract_iids.sh
	$(srcdir)/extract_iids.sh $< >$@

IBakery_GUIDs.cpp: IBakery_idl.h extract_iids.sh
	$(srcdir)/extract_iids.sh $< >$@

IWall_GUIDs.cpp: IWall.h extract_iids.sh
	$(srcdir)/extract_iids.sh $< >$@

# TODO do in a way that isn't a dirty hack with manually removing IBall,Ball GUID definitions from IBag_GUIDs.cpp since IBagLib.idl #includes IBall.idh
IBag_GUIDs.cpp: IBag.h extract_iids.sh
	$(srcdir)/extract_iids.sh $< >$@
	TMPFILE="$$(mktemp -t)" && \
	mv IBag_GUIDs.cpp "$$TMPFILE" && \
	grep -v -e "__uuidof(IBall)" -e "__uuidof(Ball)" "$$TMPFILE" > IBag_GUIDs.cpp

IWall.tlb IWall.h: IWall.idl fix-midl.sh
	$(MIDL) "$(REMOTE_WIN32_DEV_HOST)" /I $(midlIncludeDir) /Oicf /env win32 /tlb IWall.tlb /h IWall.h $<
	TMPFILE="$$(mktemp -t)" && \
	mv IWall.h "$$TMPFILE" && \
	$(srcdir)/fix-midl.sh "$$TMPFILE" > IWall.h

IWallInterop.dll: IWall.tlb
	$(TLBIMP) "$(REMOTE_WIN32_DEV_HOST)" $< /out:IWallInterop.dll

# TODO not dirty hack
IBag.tlb IBag.h: IBagLib_.idl fix-midl.sh
	$(MIDL) "$(REMOTE_WIN32_DEV_HOST)" /I $(midlIncludeDir) /Oicf /env win32 /tlb IBagLib_.tlb /h IBagLib_.h $<
	mv IBagLib_.tlb IBag.tlb
	mv IBagLib_.h IBag.h
	TMPFILE="$$(mktemp -t)" && \
	mv IBag.h "$$TMPFILE" && \
	$(srcdir)/fix-midl.sh "$$TMPFILE" > IBag.h

IBagInterop.dll: IBag.tlb
	$(TLBIMP) "$(REMOTE_WIN32_DEV_HOST)" $< /out:IBagInterop.dll

IBagLib_.idl: IBagLib.idl IBag.idh IBall.idh
	gcc -E -P -x c -o $@ $<

# Auto-generate Jar_GUIDs.cpp when the script handles it properly
#Jar_GUIDs.cpp: jar.tlh extract_iids.sh
#	$(srcdir)/extract_iids.sh $< >$@

Ball.lo: IBall_idl.h
Bakery.lo: IBakery_idl.h
Wall.lo: IWall.h
Bag.lo: IBag.h

IBallMetadata.dll:
	ln -sf $(srcdir)/raw-midl/$@

IBakeryMetadata.dll:
	ln -sf $(srcdir)/raw-midl/$@

components.map: bakery.la ball.la wall.la bag.la
	LD_LIBRARY_PATH=$(FieldWorksRootDir)/Lib/src/icu/install$(FWARCH)/lib:$(abs_top_builddir)/src/.libs \
	PATH=$(abs_top_srcdir)/bin$(FWARCH):$(PATH) \
	make-map.sh .libs/bakery.so .libs/ball.so .libs/wall.so .libs/bag.so

check_PROGRAMS = components.map

TESTS = .libs/comtest ComClient.exe BakeryJarTestClient.exe Tester.exe
TESTS_ENVIRONMENT = COMPONENTS_MAP_PATH=. LD_LIBRARY_PATH=:.libs:../src/.libs:$$($(IcuConfig) --libdir)

MOSTLYCLEANFILES = IBall_idl.h IBall_GUIDs.cpp ComClient.exe.mdb Tester.exe.mdb IBakery_idl.h IBakery_GUIDs.cpp IWall_GUIDs.cpp IBag_GUIDs.cpp
CLEANFILES = IBallMetadata.dll IBakeryMetadata.dll Jar.dll Jar.dll.mdb BakeryJarTestClient.exe.mdb IWall.h IWall.tlb IWallInterop.dll IBag.h IBag.tlb IBagInterop.dll IBagLib_.idl
if USE_FW_GENERIC_FACTORY
# Only clean components.map if the pieces are available to regenerate it.
CLEANFILES += components.map
endif

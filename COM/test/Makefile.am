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

# Ensure targets like components.map get removed if they're not generated
# completely, otherwise a second run of make check thinks they're up to date
# and doesn't try to rebuild them (eg tries to run the tests with an incomplete
# components.map)
.DELETE_ON_ERROR:

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

AUTOMAKE_OPTIONS = color-tests parallel-tests

noinst_PROGRAMS = comtest ComClient.exe BakeryJarTestClient.exe Tester.exe
noinst_LTLIBRARIES = ball.la bakery.la wall.la bag.la

INCLUDES = \
	$(all_includes) \
	-I$(srcdir)/include \
	-I$(top_srcdir)/include \
	$$($(IcuConfig) --detect-prefix --cppflags)

AM_LDFLAGS = \
	$$($(IcuConfig) --detect-prefix --ldflags) \
	$(all_libraries) \
	-luuid

if USE_FW_GENERIC_FACTORY
INCLUDES += \
	-I$(Win32BaseInc) \
	-I$(Win32MoreInc) \
	-I$(FieldWorksRootDir)/Include \
	-I$(FieldWorksRootDir)/Src/Generic
endif

MIDL = $(top_srcdir)/remote-midl/midl "$(REMOTE_WIN32_DEV_HOST)"
TLBIMP = $(top_srcdir)/remote-midl/tlbimp "$(REMOTE_WIN32_DEV_HOST)"
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
	$(FieldWorksObjDebugDir)/DebugProcs/DebugProcs.o \
	-L$(Win32BaseLib) -lWin32Base \
	-L$(Win32MoreLib) -lWin32More \
	-lpthread
endif
bag_la_LDFLAGS = -module -rpath /nowhere $(AM_LDFLAGS)


wall_la_SOURCES = \
	Wall.cpp \
	IWall_GUIDs.cpp
wall_la_LIBADD = \
	-L../src/.libs -lcom
if USE_FW_GENERIC_FACTORY
wall_la_LIBADD += \
	$(FieldWorksOutputDebugDir)/libGeneric.a \
	$(FieldWorksObjDebugDir)/Generic/ModuleEntry.o \
	$(FieldWorksObjDebugDir)/DebugProcs/DebugProcs.o \
	-L$(Win32BaseLib) -lWin32Base \
	-L$(Win32MoreLib) -lWin32More \
	-lpthread
endif
wall_la_LDFLAGS = -module -rpath /nowhere $(AM_LDFLAGS)

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
	$(FieldWorksObjDebugDir)/DebugProcs/DebugProcs.o \
	-L$(Win32BaseLib) -lWin32Base \
	-L$(Win32MoreLib) -lWin32More \
	-lpthread
endif
bakery_la_LDFLAGS = -module -rpath /nowhere $(AM_LDFLAGS)


ball_la_SOURCES = \
	Ball.cpp \
	IBall_GUIDs.cpp

ball_la_LIBADD = \
	../src/libcom.la
if USE_FW_GENERIC_FACTORY
ball_la_LIBADD += \
	$(FieldWorksOutputDebugDir)/libGeneric.a \
	$(FieldWorksObjDebugDir)/Generic/ModuleEntry.o \
	$(FieldWorksObjDebugDir)/DebugProcs/DebugProcs.o \
	-L$(Win32BaseLib) -lWin32Base \
	-L$(Win32MoreLib) -lWin32More \
	-lpthread
endif
ball_la_LDFLAGS = -module -rpath /nowhere $(AM_LDFLAGS)

comtest_SOURCES = CClient.cc
comtest_LDADD = IBall_GUIDs.o ../src/libcom.la -ldl

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
	include/comdef.h \
	include/oaidl.h \
	include/ocidl.h \
	include/rpc.h \
	include/rpcndr.h \
	raw-midl/IBag.h \
	raw-midl/IBagInterop.dll \
	raw-midl/IBakery.h \
	raw-midl/IBakeryMetadata.dll \
	raw-midl/IBall.h \
	raw-midl/IBallMetadata.dll \
	raw-midl/IWall.h \
	raw-midl/IWallInterop.dll \
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

IBall_GUIDs.cpp: IBall.h extract_iids.sh
	$(srcdir)/extract_iids.sh $< >$@

IBakery_GUIDs.cpp: IBakery.h extract_iids.sh
	$(srcdir)/extract_iids.sh $< >$@

IWall_GUIDs.cpp: IWall.h extract_iids.sh
	$(srcdir)/extract_iids.sh $< >$@

# TODO do in a way that isn't a dirty hack with manually removing IBall,Ball GUID definitions from IBag_GUIDs.cpp since IBagLib.idl #includes IBall.idh
IBag_GUIDs.cpp: IBag.h extract_iids.sh
	$(srcdir)/extract_iids.sh $< | \
	grep -v -e "DEFINE_UUIDOF(IBall," -e "DEFINE_UUIDOF(Ball," >$@

if USE_REMOTE_MIDL

IBagLib_.idl: IBagLib.idl IBag.idh IBall.idh
	gcc -E -P -x c -o $@ $<

IBall_.idl: IBall.idl IBall.idh
	gcc -E -P -x c -o $@ $<

$(srcdir)/raw-midl/IBall.h: IBall_.idl
	$(MIDL) /I $(midlIncludeDir) /nologo /Oicf /env win32 /h $@ $<

$(srcdir)/raw-midl/IBakery.h: IBakery.idl
	$(MIDL) /I $(midlIncludeDir) /nologo /Oicf /env win32 /h $@ $<

$(srcdir)/raw-midl/IWall.h IWall.tlb: IWall.idl
	$(MIDL) /I $(midlIncludeDir) /nologo /Oicf /env win32 /tlb IWall.tlb /h $(srcdir)/raw-midl/IWall.h $<

$(srcdir)/raw-midl/IBag.h IBag.tlb: IBagLib_.idl
	$(MIDL) /I $(midlIncludeDir) /nologo /Oicf /env win32 /tlb IBag.tlb /h $(srcdir)/raw-midl/IBag.h $<

$(srcdir)/raw-midl/IWallInterop.dll: IWall.tlb
	$(TLBIMP) $< /out:$@ /silent

$(srcdir)/raw-midl/IBagInterop.dll: IBag.tlb
	$(TLBIMP) $< /out:$@ /silent

endif #USE_REMOTE_MIDL

IBall.h: raw-midl/IBall.h fix-midl.sh
	$(srcdir)/fix-midl.sh $< >$@

IBakery.h: raw-midl/IBakery.h fix-midl.sh
	$(srcdir)/fix-midl.sh $< >$@

IWall.h: raw-midl/IWall.h fix-midl.sh
	$(srcdir)/fix-midl.sh $< >$@

IBag.h: raw-midl/IBag.h fix-midl.sh
	$(srcdir)/fix-midl.sh $< >$@

IWallInterop.dll: raw-midl/IWallInterop.dll
	cp $< $@

IBagInterop.dll: raw-midl/IBagInterop.dll
	cp $< $@

# Auto-generate Jar_GUIDs.cpp when the script handles it properly
#Jar_GUIDs.cpp: jar.tlh extract_iids.sh
#	$(srcdir)/extract_iids.sh $< >$@

CClient.o: IBall.h
Ball.lo: IBall.h
Bakery.lo: IBakery.h
Wall.lo: IWall.h
Bag.lo: IBag.h

IBallMetadata.dll:
	ln -sf $(srcdir)/raw-midl/$@

IBakeryMetadata.dll:
	ln -sf $(srcdir)/raw-midl/$@

components_map_SOURCES =

components.map: bakery.la ball.la wall.la bag.la
	LD_LIBRARY_PATH=$(FieldWorksRootDir)/Lib/src/icu/install$(FWARCH)/lib:$(abs_top_builddir)/src/.libs:$(LD_LIBRARY_PATH) \
	PATH=$(abs_top_builddir)/src/RegisterServer:$(abs_top_srcdir)/src/RegisterServer:$(PATH) \
	make-map.sh .libs/bakery.so .libs/ball.so .libs/wall.so .libs/bag.so

ole32_dll_SOURCES =

ole32.dll:
	ln -sf ../src/.libs/libcom.so $@

check_PROGRAMS = components.map ole32.dll

TESTS = .libs/comtest ComClient.exe BakeryJarTestClient.exe Tester.exe
TESTS_ENVIRONMENT = COMPONENTS_MAP_PATH=. LD_LIBRARY_PATH=:.libs:../src/.libs:$$($(IcuConfig) --detect-prefix --libdir)
TEST_EXTENSIONS = .exe
EXE_LOG_COMPILER = cli

MOSTLYCLEANFILES = IBall.h IBall_GUIDs.cpp ComClient.exe.mdb Tester.exe.mdb IBakery.h IBakery_GUIDs.cpp IWall_GUIDs.cpp IBag_GUIDs.cpp
CLEANFILES = IBallMetadata.dll IBakeryMetadata.dll Jar.dll Jar.dll.mdb BakeryJarTestClient.exe.mdb IWall.h IWall.tlb IWallInterop.dll IBag.h IBag.tlb IBagInterop.dll IBagLib_.idl IBall_.idl IBall.tlb IBakery.tlb
if USE_FW_GENERIC_FACTORY
# Only clean components.map if the pieces are available to regenerate it.
CLEANFILES += components.map
endif

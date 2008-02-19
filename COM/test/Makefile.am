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

if USE_FW_GENERIC_FACTORY
AM_CPPFLAGS = -DUSE_FW_GENERIC_FACTORY -DUSE_FW_MODULE_ENTRY
endif

FieldWorksRootDir = ../../../projects/FieldWorks
WIN32_HOST = gonebyebye

noinst_PROGRAMS = comtest ComClient.exe BakeryJarTestClient.exe Tester.exe
# TODO: ball.so shouldn't be installed, just built.
lib_LTLIBRARIES = ball.la bakery.la wall.la bag.la

INCLUDES = \
	$(all_includes) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../Win32Base/include \
	-I$(FieldWorksRootDir)/Include 
if USE_FW_GENERIC_FACTORY
INCLUDES += \
	-I$(FieldWorksRootDir)/Src/Generic 
endif

MIDL = $(top_srcdir)/remote-midl/midl
TLBIMP = $(top_srcdir)/remote-midl/tlbimp

bag_la_SOURCES = \
	Bag.cpp \
	IBag_GUIDs.cpp \
	IBall_GUIDs.cpp

#.libs/ball.o 
bag_la_LIBADD = \
	-L../src/.libs -lcom \
	-L../../Win32Base/src -lWin32Base
if USE_FW_GENERIC_FACTORY
bag_la_LIBADD += \
	$(FieldWorksRootDir)/Src/Generic/libGeneric.a \
	$(FieldWorksRootDir)/Src/Generic/ModuleEntry.o 
endif
bag_la_LDFLAGS = -module $(all_libraries) \
	-luuid \
	$(shell icu-config --ldflags-libsonly)


wall_la_SOURCES = \
	Wall.cpp \
	IWall_GUIDs.cpp
wall_la_LIBADD = \
	-L../src/.libs -lcom \
	-L../../Win32Base/src -lWin32Base
if USE_FW_GENERIC_FACTORY
wall_la_LIBADD += \
	$(FieldWorksRootDir)/Src/Generic/libGeneric.a \
	$(FieldWorksRootDir)/Src/Generic/ModuleEntry.o 
endif
wall_la_LDFLAGS = -module $(all_libraries) \
	-luuid \
	$(shell icu-config --ldflags-libsonly)

bakery_la_SOURCES = \
	Bakery.cpp \
	IBakery_GUIDs.cpp \
	Jar_GUIDs.cpp
bakery_la_LIBADD = \
	../src/libcom.la \
	-L../../Win32Base/src -lWin32Base 
if USE_FW_GENERIC_FACTORY
bakery_la_LIBADD += \
	$(FieldWorksRootDir)/Src/Generic/libGeneric.a \
	$(FieldWorksRootDir)/Src/Generic/ModuleEntry.o 
endif
bakery_la_LDFLAGS = -module $(all_libraries) \
	-luuid \
	$(shell icu-config --ldflags-libsonly)


ball_la_SOURCES = \
	Ball.cpp \
	IBall_GUIDs.cpp 

ball_la_LIBADD = \
	../src/libcom.la \
	-L../../Win32Base/src -lWin32Base
if USE_FW_GENERIC_FACTORY
ball_la_LIBADD += \
	$(FieldWorksRootDir)/Src/Generic/libGeneric.a \
	$(FieldWorksRootDir)/Src/Generic/ModuleEntry.o 
endif
ball_la_LDFLAGS = -module $(all_libraries) \
	-luuid \
	$(shell icu-config --ldflags-libsonly)

comtest_SOURCES = CClient.cc
comtest_LDADD = IBall_GUIDs.o ../src/libcom.la
comtest_LDFLAGS = \
	$(all_libraries) \
	-luuid \
	-ldl \
	$(shell icu-config --ldflags)

EXTRA_DIST = test.sh Jar.cs fix-midl.sh extract_iids.sh Bakery.h Wall.h Bag.h jar.tlh jar.tli Ball.h tools.h raw-midl/IBakery_idl.h raw-midl/IBakeryMetadata.dll raw-midl/IBall_idl.h raw-midl/IBall_iid.h raw-midl/IBallMetadata.dll raw-midl/IBall.tlb raw-midl/jar.tlh raw-midl/jar.tli raw-midl/README

CSC = gmcs
CSC_FLAGS = -debug
Tester_exe_SOURCES = Tester.cs
Tester_exe_REFERENCES = IWallInterop.dll IBagInterop.dll IBallMetadata.dll
ComClient_exe_SOURCES = ComClient.cs
ComClient_exe_REFERENCES = IBallMetadata.dll
BakeryJarTestClient_exe_SOURCES = BakeryJarTestClient.cs
BakeryJarTestClient_exe_REFERENCES = IBakeryMetadata.dll Jar.dll
Jar_dll_SOURCES_NonAM = Jar.cs
Jar_dll_REFERENCES_NonAM = 
Jar_dll_CSC_FLAGS_NonAM = -target:library

#IWall.idl
Tester.exe: $(Tester_exe_SOURCES) $(Tester_exe_REFERENCES)
	$(CSC) $(CSC_FLAGS) -out:$@ $(Tester_exe_SOURCES) $(Tester_exe_REFERENCES:%=-r:%)

ComClient.exe: $(ComClient_exe_SOURCES) $(ComClient_exe_REFERENCES)
	$(CSC) $(CSC_FLAGS) -out:$@ $(ComClient_exe_SOURCES) $(ComClient_exe_REFERENCES:%=-r:%)
BakeryJarTestClient.exe: $(BakeryJarTestClient_exe_SOURCES) $(BakeryJarTestClient_exe_REFERENCES)
	$(CSC) $(CSC_FLAGS) -out:$@ $(BakeryJarTestClient_exe_SOURCES) $(BakeryJarTestClient_exe_REFERENCES:%=-r:%)
Jar.dll: $(Jar_dll_SOURCES_NonAM) $(Jar_dll_REFERENCES_NonAM)
	$(CSC) $(CSC_FLAGS) $(Jar_dll_CSC_FLAGS_NonAM) -out:$@ $(Jar_dll_SOURCES_NonAM) $(Jar_dll_REFERENCES_NonAM:%=-r:%)

IBall_idl.h: raw-midl/IBall_idl.h fix-midl.sh
	./fix-midl.sh $< >$@

IBakery_idl.h: raw-midl/IBakery_idl.h fix-midl.sh
	./fix-midl.sh $< >$@

IBall_GUIDs.cpp: IBall_idl.h extract_iids.sh
	./extract_iids.sh $< >$@

IBakery_GUIDs.cpp: IBakery_idl.h extract_iids.sh
	./extract_iids.sh $< >$@

IWall_GUIDs.cpp: IWall.h extract_iids.sh
	./extract_iids.sh $< >$@

IBag_GUIDs.cpp: IBag.h extract_iids.sh
	./extract_iids.sh $< >$@

IWall.h: IWall.idl fix-midl.sh
	$(MIDL) $(WIN32_HOST) /I "C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include" /Oicf /env win32 /tlb IWall.tlb /h IWall.h IWall.idl
	TMPFILE="$(shell mktemp -t)" && \
	mv IWall.h "$$TMPFILE" && \
	./fix-midl.sh "$$TMPFILE" > IWall.h

# IWallInterop.dll actually depends on IWall.tlb, not IWall.h
IWallInterop.dll: IWall.idl IWall.h
	$(TLBIMP) $(WIN32_HOST) IWall.tlb /out:IWallInterop.dll

IBag.h: IBag.idl fix-midl.sh
	$(MIDL) $(WIN32_HOST) /I "C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include" /Oicf /env win32 /tlb IBag.tlb /h IBag.h IBag.idl
	TMPFILE="$(shell mktemp -t)" && \
	mv IBag.h "$$TMPFILE" && \
	./fix-midl.sh "$$TMPFILE" > IBag.h

# IBagInterop.dll actually depends on IBag.tlb, not IBag.h
IBagInterop.dll: IBag.idl IBag.h
	$(TLBIMP) $(WIN32_HOST) IBag.tlb /out:IBagInterop.dll


# Auto-generate Jar_GUIDs.cpp when the script handles it properly
#Jar_GUIDs.cpp: jar.tlh extract_iids.sh
#	./extract_iids.sh $< >$@

Ball.lo: IBall_idl.h
Bakery.lo: IBakery_idl.h
Wall.lo: IWall.h
Bag.lo: IBag.h

IBallMetadata.dll:
	ln -s raw-midl/$@

IBakeryMetadata.dll:
	ln -s raw-midl/$@

components.map: bakery.la ball.la wall.la bag.la
	LD_LIBRARY_PATH=$(FieldWorksRootDir)/Lib/src/icu/install/lib:$(FieldWorksRootDir)/../../libraries/COM/src/.libs PATH=$(PATH):$(FieldWorksRootDir)/../../libraries/COM/bin make-map.sh .libs/bakery.so .libs/ball.so .libs/wall.so .libs/bag.so

run-test:
	sh test.sh

MOSTLYCLEANFILES = IBall_idl.h IBall_GUIDs.cpp ComClient.exe.mdb Tester.exe.mdb IBakery_idl.h IBakery_GUIDs.cpp IWall_GUIDs.cpp IBag_GUIDs.cpp
CLEANFILES = IBallMetadata.dll IBakeryMetadata.dll Jar.dll Jar.dll.mdb BakeryJarTestClient.exe.mdb components.map IWall.h IWall.tlb IWallInterop.dll IBag.h IBag.tlb IBagInterop.dll

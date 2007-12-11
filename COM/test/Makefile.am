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

noinst_PROGRAMS = comtest ComClient.exe BakeryJarTestClient.exe
# TODO: ball.so shouldn't be installed, just built.
lib_LTLIBRARIES = ball.la bakery.la

INCLUDES = \
	$(all_includes) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../Win32Base/include \
	-I$(FieldWorksRootDir)/Include 
if USE_FW_GENERIC_FACTORY
INCLUDES += \
	-I$(FieldWorksRootDir)/Src/Generic 
endif

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

CSC = gmcs
CSC_FLAGS = -debug
ComClient_exe_SOURCES = ComClient.cs
ComClient_exe_REFERENCES = IBallMetadata.dll
BakeryJarTestClient_exe_SOURCES = BakeryJarTestClient.cs
BakeryJarTestClient_exe_REFERENCES = IBakeryMetadata.dll Jar.dll
Jar_dll_SOURCES_NonAM = Jar.cs
Jar_dll_REFERENCES_NonAM = 
Jar_dll_CSC_FLAGS_NonAM = -target:library

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

# Auto-generate Jar_GUIDs.cpp when the script handles it properly
#Jar_GUIDs.cpp: jar.tlh extract_iids.sh
#	./extract_iids.sh $< >$@

Ball.lo: IBall_idl.h
Bakery.lo: IBakery_idl.h

IBallMetadata.dll:
	ln -s raw-midl/$@

IBakeryMetadata.dll:
	ln -s raw-midl/$@

components.map: bakery.la ball.la
	LD_LIBRARY_PATH=$(FieldWorksRootDir)/Lib/src/icu/install/lib:$(FieldWorksRootDir)/../../libraries/COM/src/.libs PATH=$(PATH):$(FieldWorksRootDir)/../../libraries/COM/bin make-map.sh .libs/bakery.so .libs/ball.so

run-test:
	sh test.sh

MOSTLYCLEANFILES = IBall_idl.h IBall_GUIDs.cpp ComClient.exe.mdb IBakery_idl.h IBakery_GUIDs.cpp
CLEANFILES = IBallMetadata.dll IBakeryMetadata.dll Jar.dll Jar.dll.mdb BakeryJarTestClient.exe.mdb components.map

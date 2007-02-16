#
#	$Id$
#
#	Ball Automake file
#
#	MarkS - 2007-02-05
#
# Ball COM Support Library Test
# Copyright (C) 2007 SIL
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

FieldWorksRootDir = ../../../projects/FieldWorks

noinst_PROGRAMS = comtest ComClient.exe
lib_LTLIBRARIES = ball.la

INCLUDES= $(all_includes) \
	-I$(top_srcdir)/include -I$(top_srcdir)/../Win32Base/include \
	-I$(FieldWorksRootDir)/Include -I$(FieldWorksRootDir)/Src/Generic

ball_la_SOURCES = Ball.cpp IBall_GUIDs.cpp
ball_la_LIBADD = \
	$(FieldWorksRootDir)/Src/Generic/libGeneric.a \
	../src/libcom.la \
	-L../../Win32Base/src -lWin32Base
ball_la_LDFLAGS = -module $(all_libraries) \
	-luuid $(shell icu-config --ldflags-libsonly)

comtest_SOURCES = CClient.cc
comtest_LDADD = IBall_GUIDs.o ../src/libcom.la
comtest_LDFLAGS = $(all_libraries) \
	-luuid -ldl $(shell icu-config --ldflags-libsonly)

CSC = gmcs
CSC_FLAGS = -debug
ComClient_exe_SOURCES = ComClient.cs
ComClient_exe_REFERENCES = -r:IBallMetadata.dll

ComClient.exe: $(ComClient_exe_SOURCES) 
	$(CSC) $(CSC_FLAGS) -out:$@ $(ComClient_exe_SOURCES) $(ComClient_exe_REFERENCES)

IBall_idl.h: raw-MIDL/IBall_idl.h fix-midl.sh
	./fix-midl.sh $< >$@

IBall_GUIDs.cpp: IBall_idl.h extract_iids.sh
	./extract_iids.sh $< >$@

Ball.lo: IBall_idl.h

MOSTLYCLEANFILES = IBall_idl.h IBall_GUIDs.cpp

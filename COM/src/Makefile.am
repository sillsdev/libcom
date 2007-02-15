#
#	$Id$
#
#	COM Support Library Automake file
#
#	MarkS - 2007-02-05
#
# COM Support Library
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

lib_LTLIBRARIES = libcom.la 

libcom_la_SOURCES = COM.cpp COMInterfaces.cpp COMClasses.cpp COMSupport.cpp

# set the include path found by configure
INCLUDES = $(all_includes) -I$(top_srcdir)/include -I$(top_srcdir)/../Win32Base/include

# the library search path.
libcom_la_LDFLAGS = $(all_libraries) -luuid -ldl $(shell icu-config --ldflags-libsonly)

install-exec-hook:
	cd $(DESTDIR)/$(libdir) && ln -s libcom.so ole32.dll

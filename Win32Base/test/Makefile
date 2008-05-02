#
#	$Id$
#
#	Makefile for Win32Base test programs
#
#	Neil Mayhew - 2006-08-29
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

CXXFLAGS = -g -Wall
CPPFLAGS = -MMD -I.. -I../include -I../../COM/include $(shell icu-config --cppflags)
LDFLAGS = $(shell icu-config --ldflags) -L../../COM/src/.libs -lcom
# LDLIBS = -licucore -luuid

PROGS = TestOutputDebugString TestGUID

all: $(PROGS)

$(PROGS): %: %.o ../src/libWin32Base.a
	$(LINK.cc) -o $@ $^ $(LDLIBS)

../src/libWin32Base.a::
	@$(MAKE) -C $(@D) $(@F) -q || \
	 $(MAKE) -C $(@D) $(@F)

run-test:
	sh test.sh

clean:
	$(RM) $(PROGS) *.[od]

-include *.d

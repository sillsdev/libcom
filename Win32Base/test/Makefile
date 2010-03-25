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

ARCH = $(shell uname -m)
BUILD_DIR = build$(ARCH)
vpath %.cpp $(shell pwd)
vpath %.o $(BUILD_DIR)

CXXFLAGS = -g -Wall
CPPFLAGS = -MMD -I.. -I../include -I../../COM/include $(shell icu-config --cppflags)
LDFLAGS = $(shell icu-config --ldflags) -L../../COM/$(BUILD_DIR)/src/.libs -lcom
# LDLIBS = -licucore -luuid

PROGS = $(BUILD_DIR)/TestOutputDebugString $(BUILD_DIR)/TestGUID $(BUILD_DIR)/TestHacks \
	$(BUILD_DIR)/TestMessageBox

all: setup $(PROGS)

setup:
	-mkdir -p $(BUILD_DIR)
	cp messageboxdouble $(BUILD_DIR)

$(PROGS): $(BUILD_DIR)/%: %.o ../src/$(BUILD_DIR)/libWin32Base.a
	$(LINK.cc) -o $@ $(BUILD_DIR)/$(<F) ../src/$(BUILD_DIR)/libWin32Base.a $(LDLIBS) 

../src/$(BUILD_DIR)/libWin32Base.a::
	@$(MAKE) -C $(@D) $(@F) -q || \
	 $(MAKE) -C $(@D) $(@F)

run-test: all
	sh test.sh

clean:
	$(RM) $(addprefix $(BUILD_DIR)/, $(PROGS)) $(BUILD_DIR)/*.[od]
	-rm -rf $(BUILD_DIR)

-include $(BUILD_DIR)/*.d

.cpp.o:
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c -o $(BUILD_DIR)/$@ $< 


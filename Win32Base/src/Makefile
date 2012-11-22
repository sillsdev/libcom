#
#	$Id$
#
#	Makefile
#
#	Neil Mayhew - 2006-08-10
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

# GCC46 is 1 if running GCC >= 4.6, otherwise 0
GCC46 := $(shell expr `gcc -dumpversion | cut -f1-2 -d.` \>= 4.6)

CPPFLAGS = $(shell icu-config --cppflags) -MMD -I../include
LDLIBS   = $(shell icu-config --ldflags)

CXXFLAGS := -Wall -g -O0 -fPIC -fvisibility=hidden -Werror

ifeq "$(GCC46)" "1"
CXXFLAGS := $(CXXFLAGS) -Wno-error=unused-but-set-variable
endif

ARFLAGS = -crs

ARCH = $(shell uname -m)
BUILD_DIR = build$(ARCH)

MODS = Hacks.o WinBase.o MessageBox.o
OBJS = $(MODS:%=$(BUILD_DIR)/%)
DEPS = $(OBJS:%.o=%.d)

all: $(BUILD_DIR)/libWin32Base.a

$(BUILD_DIR)/libWin32Base.a: $(OBJS)
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(COMPILE.cc) -o $@ $<

clean:
	$(RM) -rf $(BUILD_DIR)

check:

-include $(DEPS)

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

CPPFLAGS = \
	-MMD \
	$(shell icu-config --cppflags) \
	-I../include \

CXXFLAGS = -Wall -g -O0 -fPIC -fvisibility=hidden -Werror

ARFLAGS = -crs

ARCH = $(shell uname -m)
BUILD_DIR = build$(ARCH)
VPATH = $(BUILD_DIR)

OBJS = Hacks.o WinBase.o
SRCS = $(OBJS:%.o=%.cpp)
DEPS = $(OBJS:%.o=%.d)

setup:
	-mkdir -p $(BUILD_DIR)

libWin32Base.a: setup $(OBJS)
	$(AR) $(ARFLAGS) $(BUILD_DIR)/$@ $(addprefix $(BUILD_DIR)/, $(OBJS))

clean:
	$(RM) $(BUILD_DIR)/libWin32Base.a $(addprefix $(BUILD_DIR)/, $(OBJS)) $(addprefix $(BUILD_DIR)/, $(DEPS))
	-rm -rf $(BUILD_DIR)

-include $(addprefix $(BUILD_DIR)/, $(DEPS))

$(BUILD_DIR)/%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<


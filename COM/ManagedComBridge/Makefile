#
#	$Id$
#
#	Makefile
#
#	Tom Hindle - 2009-01-15
#
# Managed COM Bridge
# Copyright (C) 2009 SIL International
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

LIBRARIES = ../..

CPPFLAGS = -MMD $(shell icu-config --cppflags) -I../include -I../../COM/include -I../../COM/src -I../../Win32Base/include \
	-I../../Win32More/include $(shell pkg-config --cflags mono)
CXXFLAGS = -Wall -g -O0 -fPIC -fvisibility=hidden -Werror

PACKAGES = mono uuid

LDFLAGS = 	$(shell pkg-config --libs $(PACKAGES)) \
			-L$(LIBRARIES)/Win32More/src/build$(ARCH) -lWin32More \
			-L$(LIBRARIES)/COM/build$(ARCH)/src/.libs -lcom \
			-L$(LIBRARIES)/Win32Base/src/build$(ARCH) -lWin32Base \
			$(shell icu-config --ldflags) 

ARCH = $(shell uname -m)
BUILD_DIR = build$(ARCH)
VPATH = $(BUILD_DIR)

OBJS = \
	ManagedComBridge.o \

SRCS = $(OBJS:%.o=%.cpp)
DEPS = $(OBJS:%.o=%.d)

all: libManagedComBridge.so

libManagedComBridge.so: setup $(addprefix $(BUILD_DIR)/, $(OBJS))
	$(LINK.cc) -shared -o $(BUILD_DIR)/$@ $(addprefix $(BUILD_DIR)/, $(OBJS)) $(LDFLAGS)

setup:
	-mkdir -p $(BUILD_DIR)

clean:
	$(RM) $(BUILD_DIR)/libManagedComBridge.so $(BUILD_DIR)/$@ $(addprefix $(BUILD_DIR)/, $(OBJS)) \
		$(BUILD_DIR)/$@ $(addprefix $(BUILD_DIR)/, $(DEPS))
	-rm -rf $(BUILD_DIR)

-include $(addprefix $(BUILD_DIR)/, $(DEPS))

$(BUILD_DIR)/%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

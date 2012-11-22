#
#	Makefile for ManagedComBridge
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

PACKAGES = mono-2 uuid glib-2.0

CPPFLAGS = -MMD \
	-I../include -I../src \
	$(shell pkg-config --cflags $(PACKAGES))

CXXFLAGS = -g -O0 -Werror -Wall -Wextra -fPIC -fvisibility=hidden

LDLIBS = \
	-L../build$(ARCH)/src/.libs -lcom \
	$(shell pkg-config --libs $(PACKAGES))

ARCH = $(shell uname -m)
BUILD_DIR = build$(ARCH)

MODS = ManagedComBridge.o
OBJS = $(MODS:%=$(BUILD_DIR)/%)
DEPS = $(OBJS:%.o=%.d)

all: $(BUILD_DIR)/libManagedComBridge.so

$(BUILD_DIR)/libManagedComBridge.so: $(OBJS)
	@mkdir -p $(@D)
	$(LINK.cc) -shared -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(COMPILE.cc) -o $@ $<

clean:
	-$(RM) -rf $(BUILD_DIR)

-include $(DEPS)

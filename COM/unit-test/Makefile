#
# Makefile for COM unit-test programs
#
# Original author: Neil Mayhew <neil_mayhew@sil.org>
# Creation date: 2010-05-12
#
# COM Support Library
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

export LD_LIBRARY_PATH := ../$(BUILD_DIR)/src/.libs:$(LD_LIBRARY_PATH)

CXXFLAGS ?= -g -O0
CXXFLAGS += -Wall -Wextra -Werror -MMD
CPPFLAGS += -I../include
LDFLAGS  += -L../$(BUILD_DIR)/src/.libs -lcom
LDLIBS = -lboost_unit_test_framework

PROGNAMES = TestCOM
PROGS = $(PROGNAMES:%=$(BUILD_DIR)/%)

all: $(PROGS)

$(BUILD_DIR)/%: %.cpp
	@mkdir -p $(@D)
	$(LINK.cc) -o $@ $< $(LDLIBS)

check: all
	for P in $(PROGS); do $$P; done

clean:
	$(RM) -rf $(BUILD_DIR)

-include $(BUILD_DIR)/*.d

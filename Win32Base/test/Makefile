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

CXXFLAGS = -g -Wall
CPPFLAGS = $(shell icu-config --cppflags) -MMD -I.. -I../include
LDLIBS   = $(shell icu-config --ldflags)

PROGNAMES   = TestOutputDebugString TestHacks TestMessageBox
SCRIPTNAMES = messageboxdouble
PROGS       = $(PROGNAMES:%=$(BUILD_DIR)/%)
SCRIPTS     = $(SCRIPTNAMES:%=$(BUILD_DIR)/%)

WIN32BASEDIR = ../src
WIN32BASELIB = $(WIN32BASEDIR)/$(BUILD_DIR)/libWin32Base.a

all: $(PROGS) $(SCRIPTS)

$(BUILD_DIR)/%: %.cpp $(WIN32BASELIB)
	@mkdir -p $(@D)
	$(LINK.cc) -o $@ $< $(WIN32BASELIB) $(LDLIBS)

$(BUILD_DIR)/%: %
	cp -p $< $@

$(WIN32BASELIB)::
	@$(MAKE) -C $(WIN32BASEDIR) -q || \
	 $(MAKE) -C $(WIN32BASEDIR)

check run-test: all
	sh test.sh $(PROGNAMES)

clean:
	$(RM) -rf $(BUILD_DIR)

-include $(BUILD_DIR)/*.d

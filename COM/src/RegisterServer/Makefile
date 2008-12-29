#
#	$Id$
#
#	Makefile for RegisterServer directory
#
#	Neil Mayhew - 2007-07-31
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

CXXFLAGS = -g -O3

ARCH = $(shell uname -m)
INSTALL_DIR = ../../bin$(ARCH)
OBJ_DIR = obj$(ARCH)

all: setup $(OBJ_DIR)/RegisterServer

setup:
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/RegisterServer: $(OBJ_DIR)/RegisterServer.o
	$(LINK.cc) -o $@ $^ -ldl

$(OBJ_DIR)/RegisterServer.o: RegisterServer.cpp
	$(COMPILE.cc) $(CXXFLAGS) -o $(OBJ_DIR)/RegisterServer.o RegisterServer.cpp

-include *.d

install: $(OBJ_DIR)/RegisterServer
	mkdir -p $(INSTALL_DIR)
	install $(OBJ_DIR)/RegisterServer make-map.sh generate-map.sh $(INSTALL_DIR)

uninstall:
	rm -f $(INSTALL_DIR)/RegisterServer
	rm -f $(INSTALL_DIR)/make-map.sh
	rm -f $(INSTALL_DIR)/generate-map.sh

clean:
	$(RM) $(OBJ_DIR)/RegisterServer $(OBJ_DIR)/*.[od] $(OBJ_DIR)/*.gch
	-rmdir $(OBJ_DIR)

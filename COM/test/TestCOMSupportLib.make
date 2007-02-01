# Makefile for COMSupportLib
#
# Graeme Costin 2003-06-20
#
# $Id$
#
#	Adapted to include Linux
#	2003-06-24, NJHM
#       Copied and modified for COMSupportLibTest directory
#	2006-12-22, MarkS

OS=$(shell uname -s)

ifeq ($(OS), Darwin)
PLATFORM_DEFINES = -DTARGET_RT_MAC_MACHO
DYNAMICLINK = -dynamiclib
LIBSUFFIX = dylib
else
PLATFORM_DEFINES =
DYNAMICLINK = -shared
LIBSUFFIX = so
endif

COMPONENT_DIR = TestComponent
COMPONENTS_DIR= Components
COMPONENT = $(COMPONENT_DIR)/Component.$(LIBSUFFIX)
CPPFLAGS = -D_DEBUG -I../src -I../include -I../../include -I$(COMPONENT_DIR) -DOS_$(OS) $(PLATFORM_DEFINES)
CXXFLAGS = -g -MMD

all: $(COMPONENT) TestCOMSupportLib

$(COMPONENT): $(COMPONENT_DIR)/Component.o $(COMPONENT_DIR)/ComponentGUIDs.o ../src/COMSupportLib.$(LIBSUFFIX)
	$(LINK.cc) $(DYNAMICLINK) $^ -o $@

TestCOMSupportLib: TestCOMSupportLib.o $(COMPONENT_DIR)/ComponentGUIDs.o ../src/COMSupportLib.$(LIBSUFFIX)
	$(LINK.cc) $^ -ldl -o $@
	mkdir -p $(COMPONENTS_DIR)
	cp -a $(COMPONENT) $(COMPONENTS_DIR)/
	
clean:
	rm -f *.[od] $(COMPONENT) TestCOMSupportLib

-include *.d

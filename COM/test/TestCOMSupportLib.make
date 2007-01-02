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

COMPONENT_DIR = ComponentDLLDebug
COMPONENTS_DIR= Components
CPPFLAGS = -D_DEBUG -I../../include -IComponentDLLDebug -DOS_$(OS) $(PLATFORM_DEFINES)
CXXFLAGS = -g

#all: COMSupportLib.$(LIBSUFFIX) Components/Component.$(LIBSUFFIX) TestCOMSupportLib
all: ComponentDLLDebug/Component.$(LIBSUFFIX) TestCOMSupportLib

#COMSupportLib.$(LIBSUFFIX): COMSupport.o
#	$(LINK.cc) $(DYNAMICLINK) $^ -o $@

$(COMPONENT_DIR)/Component.$(LIBSUFFIX): $(COMPONENT_DIR)/Component.o $(COMPONENT_DIR)/ComponentGUIDs.o ../COMSupportLib.$(LIBSUFFIX)
	$(LINK.cc) $(DYNAMICLINK) $^ -o $@

TestCOMSupportLib: TestCOMSupportLib.o $(COMPONENT_DIR)/ComponentGUIDs.o ../COMSupportLib.$(LIBSUFFIX)
	$(LINK.cc) $^ -ldl -o $@
	cp -a $(COMPONENT_DIR)/Component.so $(COMPONENTS_DIR)/
	
clean:
	rm -f *.o ComponentDLLDebug/Component.$(LIBSUFFIX) TestCOMSupportLib

depend .depend:
	mkdep $(CPPFLAGS) *.cpp

include .depend

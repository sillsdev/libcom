# Makefile for COMSupportLib
#
# Graeme Costin 2003-06-20
#
# $Id$
#
#	Adapted to include Linux
#	2003-06-24, NJHM

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

CPPFLAGS = -D_DEBUG -I../include -DOS_$(OS) $(PLATFORM_DEFINES)

all: COMSupportLib.$(LIBSUFFIX) Components/Component.$(LIBSUFFIX) TestCOMSupportLib

COMSupportLib.$(LIBSUFFIX): COMSupport.o
	$(LINK.cc) $(DYNAMICLINK) $^ -o $@

Components/Component.$(LIBSUFFIX): Component.o ComponentGUIDs.o COMSupportLib.$(LIBSUFFIX)
	$(LINK.cc) $(DYNAMICLINK) $^ -o $@

TestCOMSupportLib: TestCOMSupportLib.o ComponentGUIDs.o COMSupportLib.$(LIBSUFFIX)
	$(LINK.cc) $^ -ldl -o $@
	
install: COMSupportLib.$(LIBSUFFIX)
	cp -p $^ ../bin

clean:
	rm -f *.o COMSupportLib.$(LIBSUFFIX) Components/Component.$(LIBSUFFIX) TestCOMSupportLib

depend .depend:
	mkdep $(CPPFLAGS) *.cpp

include .depend

# Makefile for COMSupportLib
#
# Graeme Costin 2003-06-20
#
# $Id$
#
#	Adapted to include Linux
#	2003-06-24, NJHM
#       Modified to move instructions to build the test to another Makefile
#       2006-12-22, MarkS
#	Linking ole32.dll to the .so file
#	2007-01-02, MarkS

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

CPPFLAGS = -D_DEBUG -I../include -I../../Win32Base/include -DOS_$(OS) $(PLATFORM_DEFINES)
CXXFLAGS = -g -MMD

all: COMSupportLib.$(LIBSUFFIX) ole32.dll

ole32.dll: COMSupportLib.$(LIBSUFFIX)
	ln -s COMSupportLib.$(LIBSUFFIX) ole32.dll

COMSupportLib.$(LIBSUFFIX): COMSupport.o
	$(LINK.cc) $(DYNAMICLINK) $^ -o $@

install: COMSupportLib.$(LIBSUFFIX)
	cp -p $^ ../bin

clean:
	rm -f *.[od] COMSupportLib.$(LIBSUFFIX)

-include *.d

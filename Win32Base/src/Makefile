CXXFLAGS = -g
ARFLAGS = -rs

libWin32SDK.a: COM.o COMClasses.o Hacks.o
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) libWin32SDK.a COM.o COMClasses.o Hacks.o

CXXFLAGS = -g
ARFLAGS = -rs

OBJS = COM.o COMClasses.o Hacks.o
SRCS = $(OBJS:%.o=%.cpp)

libWin32SDK.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) libWin32SDK.a $(OBJS) .depend

.depend: $(SRCS)
	@$(COMPILE.cc) -MM $(SRCS) >$@

-include .depend

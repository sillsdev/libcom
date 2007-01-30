CPPFLAGS = -MMD
CXXFLAGS = -g -Wall
ARFLAGS = -rs

OBJS = COM.o COMClasses.o Hacks.o TextServices.o
SRCS = $(OBJS:%.o=%.cpp)
DEPS = $(OBJS:%.o=%.d)

libWin32SDK.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) libWin32SDK.a $(OBJS) $(DEPS)

-include $(DEPS)

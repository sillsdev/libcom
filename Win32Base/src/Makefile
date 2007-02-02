CPPFLAGS = -MMD -I../include
CXXFLAGS = -g -Wall
ARFLAGS = -rs

OBJS = Hacks.o
SRCS = $(OBJS:%.o=%.cpp)
DEPS = $(OBJS:%.o=%.d)

libWin32SDK.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) libWin32SDK.a $(OBJS) $(DEPS)

-include $(DEPS)

CPPFLAGS = -MMD -I../include -I../../COM/include -I../../Win32Base/include
CXXFLAGS = -g -Wall
ARFLAGS = -rs

OBJS = TextServices.o
SRCS = $(OBJS:%.o=%.cpp)
DEPS = $(OBJS:%.o=%.d)

libWin32More.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) libWin32More.a $(OBJS) $(DEPS)

-include $(DEPS)

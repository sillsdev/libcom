CPPFLAGS = \
	-MMD \
	-I../include \
	$(shell icu-config --cppflags) \

CXXFLAGS = -g -Wall
ARFLAGS = -rs

OBJS = Hacks.o
SRCS = $(OBJS:%.o=%.cpp)
DEPS = $(OBJS:%.o=%.d)

libWin32Base.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) libWin32Base.a $(OBJS) $(DEPS)

-include $(DEPS)

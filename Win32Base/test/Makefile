CXXFLAGS = -g
CPPFLAGS = -MMD -I..
LDLIBS = -licucore -luuid

PROGS = TestOutputDebugString TestGUID

all: $(PROGS)

$(PROGS): %: %.o ../libWin32SDK.a
	$(LINK.cc) -o $@ $^ $(LDLIBS)

../libWin32SDK.a::
	@$(MAKE) -C $(@D) $(@F) -q || \
	 $(MAKE) -C $(@D) $(@F)

clean:
	$(RM) $(PROGS) *.[od]

-include *.d

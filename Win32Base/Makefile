all: # Default target

# Recurse any target into the subdirectories

%:
	$(MAKE) -C src $@
	$(MAKE) -C test $@

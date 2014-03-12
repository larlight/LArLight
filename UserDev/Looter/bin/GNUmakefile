# Include your header file location
CXXFLAGS += -I$(USER_DEV_DIR)/Looter
CXXFLAGS += -I. $(shell root-config --cflags) -g

# Include your shared object lib location
LDFLAGS += -L$(LARLIGHT_LIBDIR) -lLooter -lBase -lDataFormat
LDFLAGS += $(shell root-config --libs) -lPhysics -lMatrix -lTreePlayer -g

INCFLAGS += -I$(LARLIGHT_CORE_DIR)/Base
INCFLAGS += -I$(LARLIGHT_CORE_DIR)/DataFormat

# platform-specific options
OSNAME = $(shell uname -s)
include $(MAKE_TOP_DIR)/Makefile/Makefile.${OSNAME}

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.
PROGRAMS = loot

all:		$(PROGRAMS)

$(PROGRAMS):
	@echo '<<compiling' $@'>>'
	@$(CXX) $@.cc -o $@ $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS)
	@rm -rf *.dSYM
clean:	
	rm -f $(PROGRAMS)

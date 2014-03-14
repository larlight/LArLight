# Include your header file location
CXXFLAGS += -I. $(shell root-config --cflags) -g

# Include your shared object lib location
LDFLAGS += -L$(LARLIGHT_LIBDIR) -lBase -lDataFormat -lLArUtil -lAnalysis
LDFLAGS += $(shell root-config --libs) -lGeom -lPhysics -lMatrix -g

# platform-specific options
OSNAME = $(shell uname -s)
include $(MAKE_TOP_DIR)/Makefile/Makefile.${OSNAME}

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.
PROGRAMS = larlight

all:		$(PROGRAMS)

$(PROGRAMS):
	@echo '<<compiling' $@'>>'
	@mkdir -p ../bin
	$(CXX) $@.cc -o ../bin/$@ $(CXXFLAGS) $(LDFLAGS)
	@rm -rf ../bin/*.dSYM

clean:	
	rm -f ../bin/larlight

# Include your header file location
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/Base
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/Analysis
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/DataFormat
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/LArUtil
CXXFLAGS += -I$(USER_DEV_DIR)/ClusterRecoUtil

CXXFLAGS += -I. $(shell root-config --cflags) -g


# Include your shared object lib location
LDFLAGS += -L$(LARLIGHT_LIBDIR) -lClusterRecoUtil -lLArUtil -lDataFormat -lAnalysis -lBase
LDFLAGS += $(shell root-config --libs) -lPhysics -lMatrix -g

# platform-specific options
OSNAME = $(shell uname -s)
include $(MAKE_TOP_DIR)/Makefile/Makefile.${OSNAME}

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.
PROGRAMS = example

all:		$(PROGRAMS)

$(PROGRAMS): *.cc
	@echo '<<compiling' $@'>>'
	@$(CXX) $@.cc -o $@ $(CXXFLAGS) $(LDFLAGS)
	@rm -rf *.dSYM
clean:	
	rm -f $(PROGRAMS)

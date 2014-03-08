# Include your header file location
CXXFLAGS += -I$(ANA_PROC_DIR)/Base
CXXFLAGS += -I$(ANA_PROC_DIR)/LArUtil
CXXFLAGS += -I. $(shell root-config --cflags) -g

# Include your shared object lib location
LDFLAGS += -L$(ANA_PROC_DIR)/lib -lBase
LDFLAGS += -L$(ANA_PROC_DIR)/lib -lLArUtil 
LDFLAGS += $(shell root-config --libs) -lPhysics -lMatrix -g

# platform-specific options
OSNAME = $(shell uname -s)
include $(ANA_PROC_DIR)/Makefile/Makefile.${OSNAME}

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.
PROGRAMS = example gen_tree_Geometry gen_tree_DetectorProperties gen_tree_LArProperties

all:		$(PROGRAMS)

$(PROGRAMS):
	@echo '<<compiling' $@'>>'
	@$(CXX) $@.cc -o $@ $(CXXFLAGS) $(LDFLAGS)
	@rm -rf *.dSYM
clean:	
	rm -f $(PROGRAMS)

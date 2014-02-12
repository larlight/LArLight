
OSNAME = $(shell uname -s)
include $(ANA_PROC_DIR)/Makefile/Makefile.${OSNAME}

LD := $(subst build/,$(ROOTSYS)/build/,$(LD))
LIBDIR  = $(ANA_PROC_DIR)/lib
LDFLAGS  += -L$(ROOTSYS)/lib  $(shell root-config --libs) -g

# Include your header file location
CXXFLAGS += -I$(ANA_PROC_DIR)/Base 
CXXFLAGS += -I$(ANA_PROC_DIR)/DataFormat
CXXFLAGS += -I$(ANA_PROC_DIR)/Analysis
CXXFLAGS += -I$(ANA_PROC_DIR)/ClusterStudy
CXXFLAGS += -I. $(shell root-config --cflags) -g

# Include your shared object lib location
LDFLAGS += -L$(LIBDIR) -lBase -lDataFormat -lAnalysis -lClusterStudy

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.
PROGRAMS = run_cluster_merge

all:		$(PROGRAMS)
	@rm -rf *.dSYM
clean:	
	rm -f $(PROGRAMS)

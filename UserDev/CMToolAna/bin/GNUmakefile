# Include your header file location
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/Base
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/DataFormat
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/Analysis
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/LArUtil
CXXFLAGS += -I$(USER_DEV_DIR)/ClusterRecoUtil
CXXFLAGS += -I$(USER_DEV_DIR)/FANN
CXXFLAGS += -I$(USER_DEV_DIR)/FANN/include
CXXFLAGS += -I$(USER_DEV_DIR)/CMTool
CXXFLAGS += -I$(USER_DEV_DIR)/McshowerLookback
CXXFLAGS += -I$(USER_DEV_DIR)/CMergePerformance
CXXFLAGS += -I. $(shell root-config --cflags) -g

# Include your shared object lib location
LDFLAGS += -L$(LARLIGHT_LIBDIR) -lCMergePerformance -lMcshowerLookback -lCMTool -lFANN -lClusterRecoUtil -lLArUtil -lAnalysis -lDataFormat -lBase 
LDFLAGS += $(shell root-config --libs) -lPhysics -lMatrix -g

# platform-specific options
OSNAME = $(shell uname -s)
include $(MAKE_TOP_DIR)/Makefile/Makefile.${OSNAME}

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.
PROGRAMS = example match_eff

all:		$(PROGRAMS)

$(PROGRAMS):
	@echo '<<compiling' $@'>>'
	@$(CXX) $@.cc -o $@ $(CXXFLAGS) $(LDFLAGS)
	@rm -rf *.dSYM
clean:	
	rm -f $(PROGRAMS)

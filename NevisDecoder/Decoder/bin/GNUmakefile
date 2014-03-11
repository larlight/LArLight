
LD := $(subst build/,$(ROOTSYS)/build/,$(LD))
LDFLAGS  += -L$(ROOTSYS)/lib  $(shell root-config --libs) -g

# Include your header file location
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/Base 
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/DataFormat
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/Analysis
CXXFLAGS += -I$(DECODER_DIR)/Algorithm
CXXFLAGS += -I$(DECODER_DIR)/FileIO
CXXFLAGS += -I$(DECODER_DIR)/Decoder
CXXFLAGS += -I. $(shell root-config --cflags) -g

# platform-specific options
OSNAME = $(shell uname -s)
include $(DECODER_DIR)/Makefile/Makefile.${OSNAME}

# Include your shared object lib location
LDFLAGS += -L$(LARLIGHT_LIBDIR)/lib -lBase -lDataFormat -lAnalysis 
LDFLAGS += -L$(DECODER_DIR)/lib -lAlgorithm -lFileIO -lDecoder

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.
PROGRAMS = decoder xmit_search_event

all:		$(PROGRAMS)
	@rm -rf *.dSYM
clean:	
	rm -f $(PROGRAMS)

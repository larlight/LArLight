
LD := $(subst build/,$(ROOTSYS)/build/,$(LD))
LDFLAGS  += -L$(ROOTSYS)/lib  $(shell root-config --libs) -g

# Include your header file location
CXXFLAGS += -I$(ANA_PROC_DIR)/Base 
CXXFLAGS += -I$(ANA_PROC_DIR)/DataFormat
CXXFLAGS += -I$(ANA_PROC_DIR)/Analysis
CXXFLAGS += -I$(PMT_DECODER_DIR)/Algorithm
CXXFLAGS += -I$(PMT_DECODER_DIR)/FileIO
CXXFLAGS += -I$(PMT_DECODER_DIR)/Decoder
CXXFLAGS += -I. $(shell root-config --cflags) -g


# Include your shared object lib location
LDFLAGS += -L$(ANA_PROC_DIR)/lib -lBase -lDataFormat -lAnalysis 
LDFLAGS += -L$(PMT_DECODER_DIR)/lib -lAlgorithm -lFileIO -lDecoder

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.
PROGRAMS = decoder xmit_search_event

all:		$(PROGRAMS)
	@rm -rf *.dSYM
clean:	
	rm -f $(PROGRAMS)

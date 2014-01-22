
LD := $(subst build/,$(ROOTSYS)/build/,$(LD))
LDFLAGS  += -L$(ROOTSYS)/lib  $(shell root-config --libs) -g

# Include your header file location
CXXFLAGS += -I$(ANA_PROC_DIR)/Base 
CXXFLAGS += -I$(ANA_PROC_DIR)/DataFormat
CXXFLAGS += -I$(DECODER_DIR)/EventBuilder
CXXFLAGS += -I. $(shell root-config --cflags) -g


# Include your shared object lib location
LDFLAGS += -L$(ANA_PROC_DIR)/lib -lBase -lDataFormat -lAnalysis 
LDFLAGS += -L$(DECODER_DIR)/lib -lBase -lDataFormat -lEventBuilder

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.
PROGRAMS = evb

all:		$(PROGRAMS)
	@rm -rf *.dSYM
clean:	
	rm -f $(PROGRAMS)

#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = DecoderCint
SHLIB = libDecoder.so
SOURCES = $(wildcard *.cc)
HEADERS = $(wildcard *.hh)
OBJECTS = $(SOURCES:.cc=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += -I$(ANA_PROC_DIR)/Base
INCFLAGS += -I$(ANA_PROC_DIR)/DataFormat
INCFLAGS += -I$(ANA_PROC_DIR)/Analysis
INCFLAGS += -I$(DECODER_DIR)/Algorithm
INCFLAGS += -I$(DECODER_DIR)/FileIO


# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

include $(DECODER_DIR)/Makefile/Makefile.${OSNAME}

# set compiler options for ROOT
CXXFLAGS +=  `root-config --cflags`

# call the common GNUmakefile
include $(DECODER_DIR)/Makefile/GNUmakefile.CORE




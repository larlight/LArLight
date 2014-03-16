#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = LarlightArgoBackendCint
SHLIB = libLarlightArgoBackend.so
SOURCES = $(wildcard *.cc)
HEADERS = $(wildcard *.hh) JsonElement.h
OBJECTS = $(SOURCES:.cc=.o)

# include options for this package
INCFLAGS = -I.                       #Include itself
INCFLAGS = -I/opt/X11/include        # png library
INCFLAGS += -I$(LARLIGHT_CORE_DIR)/Base
INCFLAGS += -I$(LARLIGHT_CORE_DIR)/DataFormat


# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

include $(MAKE_TOP_DIR)/Makefile/Makefile.${OSNAME}

# set compiler options for ROOT
CXXFLAGS +=  `root-config --cflags`

# call the common GNUmakefile
include $(MAKE_TOP_DIR)/Makefile/GNUmakefile.CORE




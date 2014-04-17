#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = OpticalDetectorCint
SHLIB = libOpticalDetector.so
SOURCES = $(wildcard *.cc)
HEADERS = $(wildcard *.hh)
OBJECTS = $(SOURCES:.cc=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += -I$(USER_DEV_DIR)/TimeService

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

include $(MAKE_TOP_DIR)/Makefile/Makefile.${OSNAME}

# set compiler options for ROOT
CXXFLAGS +=  `root-config --cflags`

# call the common GNUmakefile
include $(MAKE_TOP_DIR)/Makefile/GNUmakefile.CORE




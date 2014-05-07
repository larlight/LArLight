#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = UBOnlineDBICint
SHLIB = libUBOnlineDBI.so
SOURCES = $(wildcard *.cxx)
HEADERS = $(wildcard *.hh)
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS = -I.                       #Include itself
INCFLAGS  += -I$(MY_PGSQL)/include

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

include $(MAKE_TOP_DIR)/Makefile/Makefile.${OSNAME}

# set compiler options for ROOT
CXXFLAGS +=  `root-config --cflags`

# call the common GNUmakefile
include $(MAKE_TOP_DIR)/Makefile/GNUmakefile.CORE




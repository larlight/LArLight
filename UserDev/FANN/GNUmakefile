#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = FANNCint
SHLIB = libFANN.so
SOURCES = $(wildcard *.cc)
HEADERS = $(wildcard *.hh)
OBJECTS = $(SOURCES:.cc=.o)

# include options for this package
INCFLAGS = -I.                       #Include itself

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

# Deal with the FANN-2.2.0-Source folder:
FANN_DIR = FANN-2.2.0-Source
.PHONY: fanncode

fanncode:
	@$(MAKE) -C $(FANN_DIR)
	@$(MAKE) install -C $(FANN_DIR)



include $(MAKE_TOP_DIR)/Makefile/Makefile.${OSNAME}

# set compiler options for ROOT
CXXFLAGS +=  `root-config --cflags`

# call the common GNUmakefile
include $(MAKE_TOP_DIR)/Makefile/GNUmakefile.CORE

clean:
	@rm lib/*.so
	@rm lib/*.so.2
	@rm lib/*.so.2.2.0


#####################################################################################
#
# A top Makefile for building my project.
# One needs to define $LARLIGHT_CORE_DIR environment variable and set it to where this
# makefile exists. 
# One can type "make" and this builds packages that are added in $SUBDIR defined below.
# 
# The original is taken from Glenn A. Smith's example for Double Chooz experiment.
#
#####################################################################################
#
# IMPOSE CONDITION BETWEEN LARLIGHT_CORE_DIR & PWD =>
#   do not compile if PWD !=$LARLIGHT_CORE_DIR is set elsewhere
#
ifndef LARLIGHT_CORE_DIR
 LARLIGHT_CORE_DIR := $(shell cd . && pwd -P)
endif
NORMALIZED_LARLIGHT_CORE_DIR := $(shell cd ${LARLIGHT_CORE_DIR} && pwd -P)
ifneq ($(NORMALIZED_LARLIGHT_CORE_DIR), $(shell cd . && pwd -P))
 ifneq ($(wildcard ./Base/*),)
ERROR_MESSAGE := $(error Your source code detected in current dir, but LARLIGHT_CORE_DIR is not current dir.  \
   To avoid recompiling the wrong installation,\
   LARLIGHT_CORE_DIR must be set to the current directory when making.  \
   Currently it is set to ${LARLIGHT_CORE_DIR} [$(NORMALIZED_LARLIGHT_CORE_DIR)].  \
   Current directory is $(shell pwd).)
 endif
endif
export LARLIGHT_CORE_DIR
#
#####################################################################################
#
# Define directories to be compile upon a global "make"...
#
SUBDIRS :=  Base DataFormat LArUtil Analysis

#####################################################################################
#
# COMPILATION...
#
#.phony: all configure default-config clean
.phony: all clean

all:
	@for i in $(SUBDIRS); do ( echo "" && echo "Compiling $$i..." && cd $$i && $(MAKE) ) || exit $$?; done
#####################################################################################
#
# CLEANs...
#
clean:
	@for i in $(SUBDIRS); do ( echo "" && echo "Cleaning $$i..." && cd $$i && $(MAKE) clean && rm -f $(LARLIGHT_LIBDIR)/$$i.* ) || exit $$?; done

#####################################################################################
#
# DOCUMENTATION...
#
doxygen:
	@echo 'dOxygenising your code...'
	@mkdir -p $(LARLIGHT_CORE_DIR)/doc/dOxygenMyProject
	@doxygen $(LARLIGHT_CORE_DIR)/doc/doxygenMyProject.script

doxygen+:
	@echo 'dOxygenising MyProject + local-ROOT...'
	@mkdir -p $(LARLIGHT_CORE_DIR)/doc/dOxygenMyProject+
	@doxygen $(LARLIGHT_CORE_DIR)/doc/doxygenMyProject+.script
#
#####################################################################################
#EOF

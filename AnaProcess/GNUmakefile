#####################################################################################
#
# A top Makefile for building my project.
# One needs to define $ANA_PROC_DIR environment variable and set it to where this
# makefile exists. 
# One can type "make" and this builds packages that are added in $SUBDIR defined below.
# 
# The original is taken from Glenn A. Smith's example for Double Chooz experiment.
#
#####################################################################################
#
# IMPOSE CONDITION BETWEEN ANA_PROC_DIR & PWD =>
#   do not compile if PWD !=$ANA_PROC_DIR is set elsewhere
#
ifndef ANA_PROC_DIR
 ANA_PROC_DIR := $(shell cd . && pwd -P)
endif
NORMALIZED_ANA_PROC_DIR := $(shell cd ${ANA_PROC_DIR} && pwd -P)
ifneq ($(NORMALIZED_ANA_PROC_DIR), $(shell cd . && pwd -P))
 ifneq ($(wildcard ./Base/*),)
ERROR_MESSAGE := $(error Your source code detected in current dir, but ANA_PROC_DIR is not current dir.  \
   To avoid recompiling the wrong installation,\
   ANA_PROC_DIR must be set to the current directory when making.  \
   Currently it is set to ${ANA_PROC_DIR} [$(NORMALIZED_ANA_PROC_DIR)].  \
   Current directory is $(shell pwd).)
 endif
endif
export ANA_PROC_DIR
#
#####################################################################################
#
# Define directories to be compile upon a global "make"...
#
SUBDIRS := Base DataFormat Analysis 

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
	@for i in $(SUBDIRS); do ( echo "" && echo "Cleaning $$i..." && cd $$i && $(MAKE) clean ) || exit $$?; done
	@rm -f $(ANA_PROC_DIR)/lib/lib*.so
	@rm -f $(ANA_PROC_DIR)/lib/lib*.rootmap

#####################################################################################
#
# DOCUMENTATION...
#
doxygen:
	@echo 'dOxygenising your code...'
	@mkdir -p $(ANA_PROC_DIR)/doc/dOxygenMyProject
	@doxygen $(ANA_PROC_DIR)/doc/doxygenMyProject.script

doxygen+:
	@echo 'dOxygenising MyProject + local-ROOT...'
	@mkdir -p $(ANA_PROC_DIR)/doc/dOxygenMyProject+
	@doxygen $(ANA_PROC_DIR)/doc/doxygenMyProject+.script
#
#####################################################################################
#EOF

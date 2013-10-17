#####################################################################################
#
# A top Makefile for building my project.
# One needs to define $MAKE_TOP_DIR to build the sub directories.
#
#####################################################################################
#
# IMPOSE CONDITION BETWEEN MAKE_TOP_DIR & PWD =>
#   do not compile if PWD !=$MAKE_TOP_DIR is set elsewhere
#
ifndef MAKE_TOP_DIR
 MAKE_TOP_DIR := $(shell cd . && pwd -P)
endif
NORMALIZED_MAKE_TOP_DIR := $(shell cd ${MAKE_TOP_DIR} && pwd -P)
ifneq ($(NORMALIZED_MAKE_TOP_DIR), $(shell cd . && pwd -P))
 ifneq ($(wildcard ./PMTDecoder/*),)
ERROR_MESSAGE := $(error Your source code detected in current dir, but MAKE_TOP_DIR is not current dir.  \
   To avoid recompiling the wrong installation,\
   MAKE_TOP_DIR must be set to the current directory when making.  \
   Currently it is set to ${MAKE_TOP_DIR} [$(NORMALIZED_MAKE_TOP_DIR)].  \
   Current directory is $(shell pwd).)
 endif
endif
export MAKE_TOP_DIR

all:
	@echo "Start building AnaProcess"
	@make --directory=$(ANA_PROC_DIR)
	@echo
	@echo "Exiting"

clean:
	@echo "Cleaning AnaProcess"
	@make clean --directory=$(ANA_PROC_DIR)
	@echo 
	@echo "Exiting"

#!/bin/bash

temp=make_rootmap.$$.temp

rootlibmap() {
 ROOTMAP=$1
 SOFILE=$2
 LINKDEF=$3
 shift 3
 DEPS=$*
 if [[ -e $SOFILE && -e $LINKDEF ]]; then
     rlibmap -f -o $ROOTMAP -l $SOFILE -d $DEPS -c $LINKDEF 2>> $temp
     rm -f $temp
 fi
}

######################################################
# KalekoAna gSystem->Load("KalekoAna") loads ALL OF THESE THINGS
rootlibmap libKalekoAna.rootmap libKalekoAna.so $USER_DEV_DIR/KalekoAna/LinkDef.h \
    libCMToolAna.so libCMToolBase.so
















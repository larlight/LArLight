#!/bin/bash

temp=make_rootmap.$$.temp
rm -f $temp

rootlibmap() {
 ROOTMAP=$1
 SOFILE=$2
 LINKDEF=$3
 shift 3
 DEPS=$*
 if [[ -e $SOFILE && -e $LINKDEF ]]; then
     rlibmap -f -o $ROOTMAP -l $SOFILE -d $DEPS -c $LINKDEF 2>> $temp
 fi
}

######################################################
# "simple" example package
rootlibmap libsimple.rootmap libsimple.so $ANA_PROC_DIR/simple/LinkDef.h 

######################################################
# Base
rootlibmap libBase.rootmap libBase.so $ANA_PROC_DIR/Base/LinkDef.h 

######################################################
# DataFormat
rootlibmap libDataFormat.rootmap libDataFormat.so $ANA_PROC_DIR/DataFormat/LinkDef.h \
    libTree.so libBase.so libMatrix.so libGenVector.so

######################################################
# Analysis
rootlibmap libAnalysis.rootmap libAnalysis.so $ANA_PROC_DIR/Analysis/LinkDef.h \
    libTree.so libGeom.so libGui.so libHist.so  libBase.so libDataFormat.so 



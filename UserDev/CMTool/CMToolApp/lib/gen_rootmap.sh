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
# CMTool
rootlibmap libCMToolApp.rootmap libCMToolApp.so $USER_DEV_DIR/CMTool/CMToolApp/LinkDef.h \
    libCMTAlgPriority.so libCMTAlgMerge.so libCMTAlgMatch.so libBase.so libDataFormat.so libAnalysis.so libGpad.so

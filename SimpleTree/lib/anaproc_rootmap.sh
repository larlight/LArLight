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
# Base
rootlibmap libBase.rootmap libBase.so $ANA_PROC_DIR/Base/LinkDef.h 

######################################################
# DataFormat
rootlibmap libDataFormat.rootmap libDataFormat.so $ANA_PROCC_DIR/DataFormat/LinkDef.h \
    libTree.so libBase.so 




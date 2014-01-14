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
    libGeom.so libGui.so libHist.so libDataFormat.so 

######################################################
# FEMPulseReco
rootlibmap libFEMPulseReco.rootmap libFEMPulseReco.so $ANA_PROC_DIR/FEMPulseReco/LinkDef.h \
    libAnalysis.so

######################################################
# FEMPulseStudy
rootlibmap libFEMPulseStudy.rootmap libFEMPulseStudy.so $ANA_PROC_DIR/FEMPulseStudy/LinkDef.h \
    libAnalysis.so


######################################################
# TrackStudy
rootlibmap libTrackStudy.rootmap libTrackStudy.so $ANA_PROC_DIR/TrackStudy/LinkDef.h \
    libAnalysis.so

######################################################
# ClusterStudy
rootlibmap libClusterStudy.rootmap libClusterStudy.so $ANA_PROC_DIR/ClusterStudy/LinkDef.h \
    libClusterStudy.so

######################################################
# KalekoAna gSystem->Load("KalekoAna") loads ALL OF THESE THINGS
rootlibmap libKalekoAna.rootmap libKalekoAna.so $ANA_PROC_DIR/KalekoAna/LinkDef.h \
    libAnalysis.so

######################################################
# KalekoTrackAna gSystem->Load("KalekoTrackAna") loads ALL OF THESE THINGS
rootlibmap libKalekoTrackAna.rootmap libKalekoTrackAna.so $ANA_PROC_DIR/KalekoTrackAna/LinkDef.h \
    libAnalysis.so

######################################################
# Georgia
rootlibmap libGeorgia.rootmap libGeorgia.so $ANA_PROC_DIR/Georgia/LinkDef.h \
    libAnalysis.so

######################################################
# ADD_NEW_ROOTMAP HERE






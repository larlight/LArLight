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
rootlibmap libBase.rootmap libBase.so $LARLIGHT_CORE_DIR/Base/LinkDef.h

######################################################
# DataFormat
rootlibmap libDataFormat.rootmap libDataFormat.so $LARLIGHT_CORE_DIR/DataFormat/LinkDef.h \
    libBase.so libPhysics.so 

######################################################
# LArUtil
rootlibmap libLArUtil.rootmap libLArUtil.so $LARLIGHT_CORE_DIR/LArUtil/LinkDef.h \
    libBase.so libHist.so libTree.so 

######################################################
# Analysis
rootlibmap libAnalysis.rootmap libAnalysis.so $LARLIGHT_CORE_DIR/Analysis/LinkDef.h \
    libDataFormat.so libHist.so  libTree.so 
#    libDataFormat.so libGeom.so libHist.so libMatrix.so libMathCore.so libGenVector.so  libTree.so 

######################################################
# FEMPulseReco
rootlibmap libFEMPulseReco.rootmap libFEMPulseReco.so $USER_DEV_DIR/FEMPulseReco/LinkDef.h \
    libAnalysis.so

######################################################
# FEMPulseStudy
rootlibmap libFEMPulseStudy.rootmap libFEMPulseStudy.so $USER_DEV_DIR/FEMPulseStudy/LinkDef.h \
    libAnalysis.so


######################################################
# TrackStudy
rootlibmap libTrackStudy.rootmap libTrackStudy.so $USER_DEV_DIR/TrackStudy/LinkDef.h \
    libAnalysis.so

######################################################
# ClusterStudy
rootlibmap libClusterStudy.rootmap libClusterStudy.so $USER_DEV_DIR/ClusterStudy/LinkDef.h \
    libClusterStudy.so

######################################################
# KalekoAna gSystem->Load("KalekoAna") loads ALL OF THESE THINGS
rootlibmap libKalekoAna.rootmap libKalekoAna.so $USER_DEV_DIR/KalekoAna/LinkDef.h \
    libAnalysis.so

######################################################
# KalekoTrackAna gSystem->Load("KalekoTrackAna") loads ALL OF THESE THINGS
rootlibmap libKalekoTrackAna.rootmap libKalekoTrackAna.so $USER_DEV_DIR/KalekoTrackAna/LinkDef.h \
    libAnalysis.so

######################################################
# Georgia
rootlibmap libGeorgia.rootmap libGeorgia.so $USER_DEV_DIR/Georgia/LinkDef.h \
    libAnalysis.so

######################################################
# TriggerSim
rootlibmap libTriggerSim.rootmap libTriggerSim.so $USER_DEV_DIR/TriggerSim/LinkDef.h \
    libAnalysis.so

######################################################
# UBOnlineDBI
rootlibmap libUBOnlineDBI.rootmap libUBOnlineDBI.so $USER_DEV_DIR/UBOnlineDBI/LinkDef.h \
    libpq

######################################################
# CoreyAna
rootlibmap libCoreyAna.rootmap libCoreyAna.so $USER_DEV_DIR/CoreyAna/LinkDef.h \
    libAnalysis.so

######################################################
# ADD_NEW_ROOTMAP HERE














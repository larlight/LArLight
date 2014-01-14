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
# FileIO
rootlibmap libFileIO.rootmap libFileIO.so $DECODER_DIR/FileIO/LinkDef.h \
    libBase.so

######################################################
# Algorithm
rootlibmap libAlgorithm.rootmap libAlgorithm.so $DECODER_DIR/Algorithm/LinkDef.h \
    libBase.so libDataFormat.so 

######################################################
# Decoder
rootlibmap libDecoder.rootmap libDecoder.so $DECODER_DIR/Decoder/LinkDef.h \
    libBase.so libDataFormat.so libAlgorithm.so libFileIO.so

######################################################
# Encoder
rootlibmap libEncoder.rootmap libEncoder.so $DECODER_DIR/Encoder/LinkDef.h \
    libBase.so libDataFormat.so libAlgorithm.so libFileIO.so

######################################################
# Pulse Reco
rootlibmap libPulseReco.rootmap libPulseReco.so $DECODER_DIR/PulseReco/LinkDef.h \
    libBase.so libDataFormat.so libAnalysis.so





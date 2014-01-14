#!/bin/bash
#
# Example setup script that you can run upon your login
#

# Step 0 Set your MAKE_TOP_DIR

export MAKE_TOP_DIR=$PWD/../../../

# Step 1: Define what to be compiled under AnaProcess repository!
#         By default, it compiles Base DataFormat and Analysis.
#         Each user should pick whatever to be compiled here.
#         Order matters (dependency!).
export ANA_PROC_MODULE="Base DataFormat Analysis Georgia"

# Step 2: Likely unnecessary ... but pick what to be compiled under
#         NevisDecoder repository. Order matters (dependency!).
export DECODER_MODULE="FileIO Algorithm Decoder"

# Step 3: Very likely unnecessary ... but pick what to be compiled
#         under LArModule repository. Order matters (dependency!).
export LAR_MODULE=""

# Step 4: Source the config script

source $MAKE_TOP_DIR/config/setup.sh

# Step 5: Compile (commented out for convenience)
#make --directory=$MAKE_TOP_DIR

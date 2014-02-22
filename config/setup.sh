#!/bin/bash

if [[ -z $MAKE_TOP_DIR ]]; then
    echo \$MAKE_TOP_DIR not set! 
    echo You have to set this first.
else 
    # Abort if ROOT not installed. Let's check rootcint for this.
    if [[ -z `command -v rootcint` ]]; then
	echo
	echo Looks like you do not have ROOT installed.
	echo You cannot use LArLight w/o ROOT!
	echo Aborting.
	echo
	return;
    fi
    LARLIGHT_OS=`uname -s`
    # Set for AnaProcess build
    if [[ -z $ANA_PROC_RELPATH ]]; then
	export ANA_PROC_RELPATH=AnaProcess
    fi
    export ANA_PROC_DIR=$MAKE_TOP_DIR/$ANA_PROC_RELPATH
    export ANA_PROC_LIBDIR=$ANA_PROC_DIR/lib
    if [[ -z $ANA_PROC_MODULE ]]; then
	export ANA_PROC_MODULE="Base DataFormat Analysis"
    fi

    # Set for NevisDecoder build
    if [[ -z $DECODER_RELPATH ]]; then
	export DECODER_RELPATH=NevisDecoder
    fi
    export DECODER_DIR=$MAKE_TOP_DIR/$DECODER_RELPATH
    export DECODER_LIBDIR=$DECODER_DIR/lib

    # Check compiler availability for clang++ and g++
    LARLIGHT_CXX=clang++
    if [ `command -v $LARLIGHT_CXX` ]; then
	export LARLIGHT_CXX;
    else
	LARLIGHT_CXX=g++
	if [[ -z `command -v $LARLIGHT_CXX` ]]; then
	    echo
	    echo Looks like you do not have neither clang or g++!
	    echo You need one of those to compile LArLight... Abort config...
	    echo
	    return;
	fi
	export LARLIGHT_CXX;
	if [ $LARLIGHT_OS = 'Darwin' ]; then
	    echo $LARLIGHT_OS
	    echo
	    echo "***************** COMPILER WARNING *******************"
	    echo "*                                                    *"
	    echo "* You are using g++ on Darwin to compile LArLight.   *"
	    echo "* Currently LArLight assumes you do not have C++11   *"
	    echo "* in this combination. Contact the author if this is *"
	    echo "* not the case. At this rate you have no ability to  *"
	    echo "* compile packages using C++11 in LArLight.          *"
	    echo "*                                                    *"
	    echo "* Help to install clang? See manual/contact author!  *"
	    echo "*                                                    *"
	    echo "******************************************************"
	    echo 
	fi
    fi
    if [[ -z $ROOTSYS ]]; then
	case `uname -n` in
	    (houston.nevis.columbia.edu)
	    if [[ -z ${ROOTSYS} ]]; then
		source /usr/nevis/adm/nevis-init.sh
		setup root
		export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH;
	    fi
	    ;;
	    (*)
	    echo
	    echo "****************** PyROOT WARNING ********************"
	    echo "*                                                    *"
	    echo "* Did not find your \$ROOTSYS. To use PyROOT feature, *"
	    echo "* Make sure ROOT.py is installed (comes with ROOT).  *"
	    echo "* You need to export \$PYTHONPATH to include the dir  *"
	    echo "* where ROOT.py exists.                              *"
	    echo "*                                                    *"
	    echo "* Help to install PyROOT? See manual/contact author! *"
	    echo "*                                                    *"
	    echo "******************************************************"
	    echo
	    ;;
	esac
    else
	export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH;
    fi

    python $MAKE_TOP_DIR/config/python/gen_anamakefile.py
    python $MAKE_TOP_DIR/config/python/gen_decodermakefile.py
    python $MAKE_TOP_DIR/config/python/gen_topmakefile.py
    export LD_LIBRARY_PATH=$ANA_PROC_LIBDIR:$DECODER_LIBDIR:$LD_LIBRARY_PATH
    if [ $LARLIGHT_OS = 'Darwin' ]; then
	export DYLD_LIBRARY_PATH=$ANA_PROC_LIBDIR:$DECODER_LIBDIR:$DYLD_LIBRARY_PATH
    fi
    echo
    echo "Finish configuration. To build, type:"
    echo "> cd \$MAKE_TOP_DIR"
    echo "> make"
    echo
fi

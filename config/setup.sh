#!/bin/bash

# Find the location of this script:
me="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Find the directory one above.
export MAKE_TOP_DIR="$( cd "$( dirname "$me" )" && pwd )"

echo "MAKE_TOP_DIR = $MAKE_TOP_DIR"
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
    # Set for core build
    export LARLIGHT_LIBDIR=$MAKE_TOP_DIR/lib
    export LARLIGHT_CORE_DIR=$MAKE_TOP_DIR/core

    # Set for user dev build
    if [[ -z $USER_DEV_RELPATH ]]; then
	export USER_DEV_RELPATH=UserDev
    fi
    export USER_DEV_DIR=$MAKE_TOP_DIR/$USER_DEV_RELPATH

    if [[ -z $USER_MODULE ]]; then
	export USER_MODULE=""
    fi

    # Set for NevisDecoder build
    if [[ -z $DECODER_RELPATH ]]; then
	export DECODER_RELPATH=NevisDecoder
    fi
    export DECODER_DIR=$MAKE_TOP_DIR/$DECODER_RELPATH

    # Check compiler availability for clang++ and g++
    LARLIGHT_CXX=clang++
    if [ `command -v $LARLIGHT_CXX` ]; then
	export LARLIGHT_CXX="clang++ -std=c++11";
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

    python $MAKE_TOP_DIR/config/python/gen_usermakefile.py
    python $MAKE_TOP_DIR/config/python/gen_decodermakefile.py
    python $MAKE_TOP_DIR/config/python/gen_topmakefile.py
    export LD_LIBRARY_PATH=$LARLIGHT_LIBDIR:$DECODER_DIR/lib:$LD_LIBRARY_PATH
    if [ $LARLIGHT_OS = 'Darwin' ]; then
	export DYLD_LIBRARY_PATH=$LARLIGHT_LIBDIR:$DECODER_LIBDIR:$DYLD_LIBRARY_PATH
    fi
    export PATH=$MAKE_TOP_DIR/bin:$PATH
    echo
    echo "Finish configuration. To build, type:"
    echo "> cd \$MAKE_TOP_DIR"
    echo "> make"
    echo
fi

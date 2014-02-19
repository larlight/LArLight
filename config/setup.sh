#!/bin/bash

if [[ -z $MAKE_TOP_DIR ]]; then
    echo \$MAKE_TOP_DIR not set! 
    echo You have to set this first.
else 
    if [[ -z $ANA_PROC_RELPATH ]]; then
	export ANA_PROC_RELPATH=AnaProcess
    fi
    export ANA_PROC_DIR=$MAKE_TOP_DIR/$ANA_PROC_RELPATH
    export ANA_PROC_LIBDIR=$ANA_PROC_DIR/lib
    if [[ -z $ANA_PROC_MODULE ]]; then
	export ANA_PROC_MODULE="Base DataFormat Analysis"
    fi


    if [[ -z $DECODER_RELPATH ]]; then
	export DECODER_RELPATH=NevisDecoder
    fi
    export DECODER_DIR=$MAKE_TOP_DIR/$DECODER_RELPATH
    export DECODER_LIBDIR=$DECODER_DIR/lib
#    if [[ -z $DECODER_MODULE ]]; then
#	export DECODER_MODULE="FileIO Algorithm Decoder"
 #   fi

    if [[ -z $ROOTSYS ]]; then
	case `uname -n` in
	    (houston.nevis.columbia.edu)
	    if [[ -z ${ROOTSYS} ]]; then
		source /usr/nevis/adm/nevis-init.sh
		setup root
	    fi
	    ;;
	    (*cmsaf*)
	    if [[ -z ${ROOTSYS} ]]; then
		export ROOTSYS=/app/d-Chooz/Software/root/root_v5.28.00
	    fi
	    ;;
	    (*)
	    echo system not recognized!!! 
	    ;;
	esac
    fi
    if [[ -z $ROOTSYS ]]; then
	echo Failed to set up ROOTSYS. Contact kazuhiro@nevis.columbia.edu
	echo
    else
	python $MAKE_TOP_DIR/config/python/gen_anamakefile.py
	python $MAKE_TOP_DIR/config/python/gen_decodermakefile.py
	python $MAKE_TOP_DIR/config/python/gen_topmakefile.py
	export LD_LIBRARY_PATH=$ANA_PROC_LIBDIR:$DECODER_LIBDIR:$LD_LIBRARY_PATH
	export DYLD_LIBRARY_PATH=$ANA_PROC_LIBDIR:$DECODER_LIBDIR:$DYLD_LIBRARY_PATH
	export PYTHONPATH=$PYTHONPATH:$ROOTSYS/lib
	echo
	echo "Finish configuration. To build, type:"
	echo "> cd \$MAKE_TOP_DIR"
	echo "> make"
	echo
    fi
fi

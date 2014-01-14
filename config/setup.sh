#!/bin/bash

if [[ -z $MAKE_TOP_DIR ]]; then
    echo \$MAKE_TOP_DIR not set! 
    echo You have to set this first.
else 
    if [[ -z $ANA_PROC_RELPATH ]]; then
	export ANA_PROC_RELPATH=AnaProcess
    fi
    export ANA_PROC_DIR=$MAKE_TOP_DIR/$ANA_PROC_RELPATH

    if [[ -z $DECODER_RELPATH ]]; then
	export DECODER_RELPATH=NevisDecoder
    fi
    export DECODER_DIR=$MAKE_TOP_DIR/$DECODER_RELPATH
	
    if [[ -z $ANA_PROC_MODULE ]]; then
	export ANA_PROC_MODULE="Base DataFormat Analysis"
    fi

    if [[ -z $DECODER_MODULE ]]; then
	export DECODER_MODULE="FileIO Algorithm Decoder"
    fi

    if [[ -z $ROOTSYS ]]; then
	case `uname -n` in
	    (*nevis.columbia.edu)
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
	export LAR_MODULE_DIR=$MAKE_TOP_DIR/LArModule
	if [[ -z ${SRT_LOCAL} ]]; then
	    export ANA_PROC_LIBDIR=$ANA_PROC_DIR/lib
	    export DECODER_LIBDIR=$DECODER_DIR/lib
	else
	    export ANA_PROC_LIBDIR=$SRT_LOCAL/lib/$SRT_SUBDIR
	    export DECODER_LIBDIR=$DECODER_DIR/lib
	    python $MAKE_TOP_DIR/config/srtlocal_clean_link.py
	    python $MAKE_TOP_DIR/config/srtlocal_make_link.py
	    rm -f $MAKE_TOP_DIR/LArModule/DataScanner/$ANA_PROC_RELPATH
	    python $MAKE_TOP_DIR/config/make_link.py $ANA_PROC_DIR  $LAR_MODULE_DIR/DataScanner/$ANA_PROC_RELPATH
	    python $MAKE_TOP_DIR/config/make_link.py $LAR_MODULE_DIR/DataScanner/src/${ANA_PROC_RELPATH}_module.cc $LAR_MODULE_DIR/DataScanner/DataScanner_module.cc
	    python $MAKE_TOP_DIR/config/make_link.py $ANA_PROC_DIR/lib/anaproc_rootmap.sh $ANA_PROC_LIBDIR/anaproc_rootmap.sh
	fi
	python $MAKE_TOP_DIR/config/gen_anamakefile.py
	python $MAKE_TOP_DIR/config/gen_decodermakefile.py
	python $MAKE_TOP_DIR/config/gen_topmakefile.py
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

#!/bin/bash

if [[ -z $MAKE_TOP_DIR ]]; then
    echo \$MAKE_TOP_DIR not set! 
    echo You have to set this first.
else 
    export ANA_PROC_DIR=$MAKE_TOP_DIR/AnaProcess
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
	if [[ -z ${SRT_LOCAL} ]]; then
	    export ANA_PROC_LIBDIR=$ANA_PROC_DIR/lib
	else
	    export ANA_PROC_LIBDIR=$SRT_LOCAL/lib/$SRT_SUBDIR
	    python $MAKE_TOP_DIR/config/srtlocal_clean_link.py
	    python $MAKE_TOP_DIR/config/srtlocal_make_link.py
	    python $MAKE_TOP_DIR/config/make_link.py $ANA_PROC_DIR  $MAKE_TOP_DIR/DataScanner/AnaProcess
	    python $MAKE_TOP_DIR/config/make_link.py $ANA_PROC_DIR/lib/make_rootmap.sh $ANA_PROC_LIBDIR/make_rootmap.sh
	fi
	python $MAKE_TOP_DIR/config/gen_topmakefile.py
	export LD_LIBRARY_PATH=$ANA_PROC_LIBDIR:$LD_LIBRARY_PATH
	export DYLD_LIBRARY_PATH=$ANA_PROC_LIBDIR:$DYLD_LIBRARY_PATH
	export PYTHONPATH=$PYTHONPATH:$ROOTSYS/lib
	echo
	echo "Finish configuration. Type:"
	echo "> make"
	echo
    fi
fi

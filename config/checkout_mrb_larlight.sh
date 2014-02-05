#!/bin/bash

command -v mrb >/dev/null 2>&1 || { echo >&2 "MRB seems not set up (required!). Aborting."; return; }

local_repo_name=kazu

if [ -z $MRB_TOP ]; then
    echo You have not set local mrb! Exiting...
    return;
else
    if [ -d $MRB_TOP/srcs/$local_repo_name ]; then
	echo You already have a directory $MRB_TOP/srcs/$local_repo_name !
	echo Doing nothing for MRB checkout...
	return;
    else
	TMP_PWD=$PWD
	cd $MRB_TOP/srcs
	echo
	echo Executing \"mrb newProduct $local_repo_name\" ...
	mrb newProduct $local_repo_name >> /dev/null
	echo
	echo Checking out from FNAL git repo...
	rm -rf $local_repo_name
	git clone ssh://p-ubooneoffline@cdcvs.fnal.gov/cvs/projects/ubooneoffline-kterao $local_repo_name
#	mrb uc
	cd $local_repo_name
	git checkout develop
	cd $TMP_PWD

	echo
	echo ... done
    fi
fi


import os,sys,commands

# check if MAKE_TOP_DIR is defined. If not, nothing to do.
if not 'MAKE_TOP_DIR' in os.environ:
    sys.exit(1)

# read in template GNUmakefile
makefile=open('%s/config/DecoderMakefile.tmp' % os.environ['MAKE_TOP_DIR'],'r').read()

# if SRT_LOCAL is not defined, generate makefile w/o LAR_PACKAGE definition
if not 'DECODER_DIR' in os.environ or not 'DECODER_MODULE' in os.environ:

    makefile=makefile.replace('DECODER_MODULE','')

else:

    packages = [x for x in os.environ['DECODER_MODULE'].split(None)]

    print ("\033[93m" + "Specified " + os.environ['DECODER_RELPATH'] + " packages to be compiled by a user:" )
    valid_packages=''
    not_found=[]
    for package in packages:

        if os.path.isdir('%s/%s' % (os.environ['DECODER_DIR'],package)):
            print ("\033[95m" + package + "\033[0m")
            valid_packages += ' %s' % package
        else:
            not_found.append("\033[91m" + package + "\033[0m")

    if not_found:
        print
        print ('\033[93m' + 'Packages not found:' + '\033[0m')
        for x in not_found:
            print x
    makefile=makefile.replace('DECODER_MODULE',valid_packages)

fout=open('%s/GNUmakefile' % os.environ['DECODER_DIR'],'w')
fout.write(makefile)
fout.close()

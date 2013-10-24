import os,sys,commands

# check if MAKE_TOP_DIR is defined. If not, nothing to do.
if not 'MAKE_TOP_DIR' in os.environ:
    sys.exit(1)

# read in template GNUmakefile
makefile=open('%s/config/Makefile.tmp' % os.environ['MAKE_TOP_DIR'],'r').read()

# if SRT_LOCAL is not defined, generate makefile w/o LAR_PACKAGE definition
if not 'SRT_LOCAL' in os.environ or not 'LAR_COMPILE' in os.environ:

    makefile=makefile.replace('LAR_COMPILE','')

else:

    packages = [x for x in os.environ['LAR_COMPILE'].split(None)]

    print "Specified LArSoft packages to be compiled by a user:"
    valid_packages=''
    not_found=[]
    for package in packages:

        if os.path.isdir('%s/%s' % (os.environ['MAKE_TOP_DIR'],package)):
            print package
            valid_packages += ' %s' % package
        else:
            not_found.append(package)

    if not_found:
        print
        print 'Packages not found:'
        for x in not_found:
            print x
    makefile=makefile.replace('LAR_COMPILE',valid_packages)

fout=open('%s/GNUmakefile' % os.environ['MAKE_TOP_DIR'],'w')
fout.write(makefile)
fout.close()

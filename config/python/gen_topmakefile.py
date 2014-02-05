import os,sys,commands

# check if MAKE_TOP_DIR is defined. If not, nothing to do.
if not 'MAKE_TOP_DIR' in os.environ:
    sys.exit(1)

# read in template GNUmakefile
makefile=open('%s/config/template/TopMakefile.tmp' % os.environ['MAKE_TOP_DIR'],'r').read()

fout=open('%s/GNUmakefile' % os.environ['MAKE_TOP_DIR'],'w')
fout.write(makefile)
fout.close()

# Simple script to build fann

import os,commands,sys

# Check cmake
cmake=commands.getoutput('which cmake')
if not cmake:
    print
    print "  ERROR: You don't seem to have cmake installed!       "
    print "         You cannot build FANN w/o cmake at the moment!"
    print "         Try installing cmake (easy on Linux/OSX!).... "
    print
    sys.exit(1)

# Check shell env. var
if not "USER_DEV_DIR" in os.environ.keys():
    print
    print "  ERROR: Source LArLight config script first!"
    print
    sys.exit(1)

# Build fann
PACKAGE_DIR = "%s/FANN" % os.environ['USER_DEV_DIR']
FANN_DIR = "%s/FANN-2.2.0-Source" % PACKAGE_DIR
cmd = "cd %s; cmake .; make" % FANN_DIR
os.system(cmd)

# Copy libraries
libs = [x for x in os.listdir("%s/src" % FANN_DIR) if x.endswith(".so") or x.endswith(".dylib")]
if not len(libs):
    print
    print "  ERROR: Building fann base package failed..."
    print
    sys.exit(1)

for l in libs:
    
    source = "%s/src/%s" % (FANN_DIR,l)
    target = "%s/lib/%s" % (os.environ['MAKE_TOP_DIR'],l.replace(".dylib",".so"))
    if os.path.isfile(target):
        #print "  INFO: %s exists ... replacing..." % l.replace(".dylib",".so")
        os.system("rm %s" % target)
        print target
    os.system("ln -s %s %s" % (source,target))
        





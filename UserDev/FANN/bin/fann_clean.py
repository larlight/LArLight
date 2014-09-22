# Simple script to clean fann
import os,commands,sys

# Check shell env. var
if not "USER_DEV_DIR" in os.environ.keys():
    print
    print "  ERROR: Source LArLight config script first!"
    print
    sys.exit(1)

# Clean library copies
PACKAGE_DIR = "%s/FANN" % os.environ['USER_DEV_DIR']
FANN_DIR = "%s/FANN-2.2.0-Source" % PACKAGE_DIR
libs = [x for x in os.listdir("%s/src" % FANN_DIR) if x.endswith(".so") or x.endswith(".dylib")]

for l in libs:
    
    source = "%s/src/%s" % (FANN_DIR,l)
    target = "%s/lib/%s" % (os.environ['MAKE_TOP_DIR'],l.replace(".dylib",".so"))
    if os.path.isfile(target):
        print target
        os.system("rm %s" % target)

os.system('make clean --directory=%s' % FANN_DIR)
        





import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(FANN)

except NameError:

    print "Failed importing FANN..."

sys.exit(0)


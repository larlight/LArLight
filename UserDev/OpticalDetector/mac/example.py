import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(OpticalDetector)

except NameError:

    print "Failed importing OpticalDetector..."

sys.exit(0)


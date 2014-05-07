import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(OpticalDetectorData)

except NameError:

    print "Failed importing OpticalDetectorData..."

sys.exit(0)


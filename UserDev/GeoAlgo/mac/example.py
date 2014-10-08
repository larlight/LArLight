import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(GeoAlgo)

except NameError:

    print "Failed importing GeoAlgo..."

sys.exit(0)


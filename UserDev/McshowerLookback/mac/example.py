import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(McshowerLookback)

except NameError:

    print "Failed importing McshowerLookback..."

sys.exit(0)


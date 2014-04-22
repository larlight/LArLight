import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(lar1Osc)

except NameError:

    print "Failed importing lar1Osc..."

sys.exit(0)


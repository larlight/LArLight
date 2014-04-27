import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(lar1Sens)

except NameError:

    print "Failed importing lar1Sens..."

sys.exit(0)


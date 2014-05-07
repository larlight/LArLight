import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(TimeService)

except NameError:

    print "Failed importing TimeService..."

sys.exit(0)


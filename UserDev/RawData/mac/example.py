import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(RawData)

except NameError:

    print "Failed importing RawData..."

sys.exit(0)


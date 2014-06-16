import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(FhiclLite)

except NameError:

    print "Failed importing FhiclLite..."

sys.exit(0)


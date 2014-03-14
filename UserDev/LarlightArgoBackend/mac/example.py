import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(LarlightArgoBackend)

except NameError:

    print "Failed importing LarlightArgoBackend..."

sys.exit(0)


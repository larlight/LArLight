import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(ClusterRecoAlg)

except NameError:

    print "Failed importing ClusterRecoAlg..."

sys.exit(0)


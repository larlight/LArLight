import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(ClusterRecoUtil)

except NameError:

    print "Failed importing ClusterRecoUtil..."

sys.exit(0)


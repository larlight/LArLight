import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(Looter)

except NameError:

    print "Failed importing Looter..."

sys.exit(0)


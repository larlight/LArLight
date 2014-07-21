import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(MinaTest)

except NameError:

    print "Failed importing MinaTest..."

sys.exit(0)


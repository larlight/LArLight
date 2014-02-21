import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(Package_Name)

except NameError:

    print "Failed importing Package_Name..."

sys.exit(0)


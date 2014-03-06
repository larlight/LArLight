import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(UBOnlineDBI)

except NameError:

    print "Failed importing UBOnlineDBI..."

sys.exit(0)


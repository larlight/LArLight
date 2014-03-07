import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(CoreyAna)

except NameError:

    print "Failed importing CoreyAna..."

sys.exit(0)


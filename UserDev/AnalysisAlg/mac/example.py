import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(AnalysisAlg)

except NameError:

    print "Failed importing AnalysisAlg..."

sys.exit(0)


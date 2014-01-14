import sys
argv=sys.argv
sys.argv=[]
import ROOT,sys
ROOT.gSystem.Load("libDecoder")
from ROOT import *

if len(argv)<3:
    print 'Usage: %s INPUT_FILE EVENT_ID'
    sys.exit(1)

k=xmit_event_search()
k.set_filename(argv[1])
k.set_target_id(int(argv[2]))
k.run()






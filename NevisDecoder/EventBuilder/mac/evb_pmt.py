import sys
from ROOT import *
gSystem.Load("libEventBuilder")
from ROOT import larlight as fmwk

evb = fmwk.event_builder()

evb.add_input(fmwk.DATA.PMTFIFO,sys.argv[1])
evb.add_input(fmwk.DATA.Trigger,sys.argv[2])

evb.set_master_stream(fmwk.DATA.Trigger)

evb.set_output_filename("%s_out.root" % sys.argv[1].replace(".root",""))

#evb.set_verbosity(fmwk.MSG.DEBUG)
#evb.set_verbosity(fmwk.MSG.INFO)
evb.set_verbosity(fmwk.MSG.NORMAL)

evb.run()

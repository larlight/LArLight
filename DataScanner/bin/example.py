import sys
from ROOT import gSystem
gSystem.Load("libDataFormat")
from ROOT import *

mgr = storage_manager()
mgr.set_verbosity(MSG.DEBUG)
mgr.add_in_filename(sys.argv[1])
mgr.set_data_to_read(DATA.Kalman3DSPS)
mgr.set_io_mode(mgr.READ)
mgr.set_in_rootdir("scanner")
mgr.open()

while mgr.next_event():
    
    track_array = mgr.get_data(DATA.Kalman3DSPS)
    
    print "Event %d .... %d tracks found!" % (track_array.event_id(),track_array.GetTrackCollection().size())

mgr.close()


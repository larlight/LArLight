import sys
from ROOT import gSystem
gSystem.Load("libDataFormat")
from ROOT import *

mgr = storage_manager()
mgr.set_verbosity(MSG.DEBUG)
mgr.add_in_filename(sys.argv[1])
mgr.set_data_to_read(DATA.Kalman3DSPS)
mgr.set_data_to_read(DATA.MCTruth)
mgr.set_io_mode(mgr.READ)
mgr.set_in_rootdir("scanner")
mgr.open()

while mgr.next_event():

    print "###################################################################################"
    print
    track_array = mgr.get_data(DATA.Kalman3DSPS)
    print "3DKalmanSPS ..."
    print "Event %d .... %d tracks found!" % (track_array.event_id(),track_array.GetTrackCollection().size())
    print
    
    mc_array = mgr.get_data(DATA.MCTruth)
    print "MCTruth ..."
    particles = mc_array.part_array()
    print "Event %d .... %d particles" % (mc_array.event_id(),particles.size())
    for x in xrange(particles.size()):
        part = particles[x]
        print "    PDGID = %d ... Track: %d ... Parent %d ... %d trajectories stored..." % (part.pdg_id(),
                                                                                            part.track_id(),
                                                                                            part.parent_id(),
                                                                                            part.step_vertex().size())
        #for y in xrange(particles[x].step_vertex().size()):
        #    print "        Step %-3d: R = (%3.2g, %3.2g, %3.2g) @ %3.2g with P = (%3.2g, %3.2g, %3.2g)" % (y,
        #                                                                                                   part.step_vertex()[y][0],
        #                                                                                                   part.step_vertex()[y][1],
        #                                                                                                   part.step_vertex()[y][2],
        #                                                                                                   part.step_time()[y],
        #                                                                                                   part.step_momentum()[y][0],
        #                                                                                                   part.step_momentum()[y][1],
        #                                                                                                   part.step_momentum()[y][2])
    print

mgr.close()


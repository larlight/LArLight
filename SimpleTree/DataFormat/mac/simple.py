from ROOT import *
gSystem.Load("libDataFormat")

k=storage_manager()
k.set_io_mode(k.WRITE)
k.set_out_filename("out.root")

k.open()
for x in xrange(100):
    k.get_data(DATA.Track)
    k.get_data(DATA.Hit)
    k.get_data(DATA.MCTruth)
    k.get_data(DATA.Cluster)
    k.get_data(DATA.SpacePoint)
    k.next_event()

k.close()


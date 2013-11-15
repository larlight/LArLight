from ROOT import *
gSystem.Load("libDataFormat")

k=storage_manager()
k.set_io_mode(k.WRITE)
k.set_out_filename("out.root")

k.open()
for x in xrange(100):
    k.get_data(DATA.Kalman3DSPS)
    k.get_data(DATA.Bezier)
    #t=k.get_tree(DATA.Bezier)
    #t.Print()
    #t.Fill()
    k.next_event()

k.close()


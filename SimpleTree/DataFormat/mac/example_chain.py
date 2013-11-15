from ROOT import *
gSystem.Load("libDataFormat")

k=storage_manager()
k.add_in_filename("out.root")
ch=k.get_chain()
#ch.Scan("bezier.entry:kalman3dsps.entry")
ch.MakeClass("aho")

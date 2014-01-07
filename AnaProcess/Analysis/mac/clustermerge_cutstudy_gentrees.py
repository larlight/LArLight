from ROOT import *
gSystem.Load("libAnalysis")
import sys

filename = "/Users/davidkaleko/Data/ShowerStudy/PDGID_11/shower_larlight_11.root"


#for studying distance cuts
#cutvals = [ 0, 2, 4, 7, 10, 30, 75, 150, 250, 300, 450, 500, 600, 700, 800, 900, 1000 ]

#for studying angle cuts
cutvals = [0, 2, 4, 7, 10, 15, 20, 25, 30, 50, 70, 90, 110, 120, 140, 160, 180]






for x in cutvals:

    ana_proc = larlight.ana_processor()
    ana_proc.set_io_mode(larlight.storage_manager.READ)
    ana_proc.add_input_file(filename)
    larlight.storage_manager.get().set_in_rootdir("scanner")
    ana_proc.set_ana_output_file("outputs/merge_Tree_cut%d.root" % x )
    my_ana = larlight.ClusterMergeAlg()
    ana_proc.add_process(my_ana)
    my_ana.SetSquaredDistanceCut( 1000000000 )
    my_ana.SetAngleCut( x )
    #if x == 2: 
    #    my_ana.VerboseMode(1)
    #ana_proc.run(0,100)

    ana_proc.run()






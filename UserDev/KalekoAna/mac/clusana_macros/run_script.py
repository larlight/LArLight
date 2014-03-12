import sys, os, ROOT
from ROOT import *
gSystem.Load("libKalekoAna")


filename = "/Users/davidkaleko/Data/ShowerStudy/PDGID_11/shower_larlight_11.root"


my_proc = larlight.ana_processor()
my_proc.set_verbosity(larlight.MSG.DEBUG)
my_proc.set_io_mode(larlight.storage_manager.READ)
my_proc.add_input_file(filename)
larlight.storage_manager.get().set_in_rootdir("scanner")
larlight.storage_manager.get().set_data_to_read(larlight.DATA.MCTruth,False)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.Shower,False)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.Calorimetry,False)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.UserInfo,False)

my_proc.set_ana_output_file("")

my_ana = kaleko.KalekoClusAna()

my_proc.add_process(my_ana)

while my_proc.process_event():
    print "Hit Enter to continue to the next event..."
    sys.stdin.readline()


# done!

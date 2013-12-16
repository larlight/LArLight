import sys, os
import ROOT
from ROOT import *
gSystem.Load("libKalekoAna")


reco_type = "Kalman3DSPS" # "Bezier" or "Kalman3DSPS"

max_evts = 10

#in_path = os.environ.get('MAKE_TOP_DIR')+"/kaleko_input_files_LArLight/"
in_path = "/a/data/morningside/kaleko"+"/kaleko_input_files_LArLight/"

in_fname = [ "v4_ang0_dsout_reduced_%s.root" % reco_type,
             "v4_ang10_dsout_reduced_%s.root" % reco_type,
             "v4_ang20_dsout_reduced_%s.root" % reco_type,
             "v4_ang30_dsout_reduced_%s.root" % reco_type,
             "v4_ang40_dsout_reduced_%s.root" % reco_type,
             "v4_ang50_dsout_reduced_%s.root" % reco_type,
             "v4_ang60_dsout_reduced_%s.root" % reco_type,
             "v4_ang70_dsout_reduced_%s.root" % reco_type,
             "v4_ang80_dsout_reduced_%s.root" % reco_type,
             "v4_ang90_dsout_reduced_%s.root" % reco_type ]

out_dir = os.environ.get('MAKE_TOP_DIR')+\
          "/AnaProcess/KalekoAna/mac/output_datatree/v4"

in_angles = [ 0, 10, 20, 30, 40, 50, 60, 70, 80, 90 ]

n_bins_histo = 100


#def main():
if not os.path.exists(out_dir):
    os.makedirs(out_dir)

mykal=KalekoTrackAna()
    
for x in xrange(len(in_fname)):

    val = mykal.EventLoop( in_path+in_fname[x],
                           int(in_angles[x]),
                           int(max_evts),
                           int(n_bins_histo) );
    print val
    print 
    print
    t = mykal.GetDataTree()
    print t
    t.SetName("tree")
    f = TFile("%s/DataTree_v4_ang%s.root" % (out_dir,in_angles[x]),"RECREATE")
    t.Write()
    f.Close()

#print 'Hit enter to exit.'
#sys.stdin.readline()

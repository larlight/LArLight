import sys, os
try:
    import ROOT
except AttributeError:
    import ROOT
from ROOT import *
gSystem.Load("libKalekoAna")


reco_type = "Kalman3DSPS" # "Bezier" or "Kalman3DSPS"

max_evts = 10

in_path = os.environ.get('MAKE_TOP_DIR')+"/kaleko_input_files_LArLight/"
in_fname = [ "v3_ang0_dsout_reduced_%s.root" % reco_type,
             "v3_ang30_dsout_reduced_%s.root" % reco_type,
             "v3_ang45_dsout_reduced_%s.root" % reco_type,
             "v3_ang60_dsout_reduced_%s.root" % reco_type,
             "v3_ang90_dsout_reduced_%s.root" % reco_type]
out_dir = os.environ.get('MAKE_TOP_DIR')+\
          "/AnaProcess/KalekoAna/mac/output_datatree/v3"

in_angles = [ 0, 30, 45, 60, 90 ]
n_bins_histo = 100


def main():
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    mykal=KalekoAna()
    
#    if reco_type == "Kalman3DSPS":
#        mykal.set_data_type(DATA.Kalman3DSPS)
#    elif reco_type == "Bezier":
#        mykal.set_data_type(DATA.Bezier)
#    else:
#        print "You fucked something up! Pick Kalman3DSPS or Bezier data type!"
#        return

    for x in xrange(len(in_fname)):

        mykal.EventLoop( in_path+in_fname[x],
                        int(in_angles[x]),
                        int(max_evts),
                        int(n_bins_histo) );

#        t = mykal.GetDataTree();
#        f = TFile("%s/DataTree_v3_ang%s.root" % (out_dir,in_angles[x]),"RECREATE")
#        t.Write()
#        f.Close()

    print 'Hit enter to exit.'
    sys.stdin.readline()



if __name__ == '__main__':
    main()

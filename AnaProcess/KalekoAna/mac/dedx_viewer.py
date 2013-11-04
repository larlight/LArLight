import sys, os
try:
    import ROOT
except AttributeError:
    import ROOT
from ROOT import *
gSystem.Load("libKalekoAna")

reco_type = "Kalman3DSPS"
max_evts = 5
n_bins_histo = 100


in_path = os.environ.get('MAKE_TOP_DIR')+"/kaleko_input_files_LArLight/"
in_fname =  "v3_ang0_dsout_reduced_%s.root" % reco_type

mykal=KalekoAna()
mykal.set_data_type(DATA.Kalman3DSPS)

mykal.EventLoop( in_path+in_fname,
                 int(0), int(max_evts), int(n_bins_histo))

tree=mykal.GetDataTree()

graph = TGraph()
canvas = TCanvas()
canvas.SetLogy()

for i in xrange(tree.GetEntries()):
    graph.Set(0)
    tree.GetEntry(i)
    graph = UtilFunctions().MakeGraphFromVectorOfPairs(tree.E_dEdx_pair,graph)
    graph.Draw("AP*")
    canvas.Update(); canvas.Modified()
    print 'Hit enter to draw the next muon\'s graph.'
    sys.stdin.readline()

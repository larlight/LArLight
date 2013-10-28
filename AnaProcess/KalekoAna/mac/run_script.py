from ROOT import *
import sys, os
gSystem.Load("libKalekoAna")


max_evts = 100
n_bins_histo = 100

in_path = "/Users/davidkaleko/Desktop/input_files_LArLight/"
in_fname = ["v3_ang0_dsout_reduced.root","v3_ang30_dsout_reduced.root","v3_ang45_dsout_reduced.root","v3_ang60_dsout_reduced.root","v3_ang90_dsout_reduced.root"]
out_dir = os.environ.get('MAKE_TOP_DIR')+"/AnaProcess/KalekoAna/mac/output_plots"
in_angles = [ 0, 30, 45, 60, 90 ]
colors = [ kOrange, kBlue, kGreen, kBlack, kMagenta ]
recohist_array = []
mchist_array = []
graph_array = []
nreco_array = []

multigraph = TMultiGraph("multigraph","Track Reco Efficiency vs. Angle Window around MC Theta")
legend = TLegend(0.59,0.18,0.83,0.38,"Legend")

def plot_shit():

    canvas.Divide(2)
    canvas.cd(1)

    for x in xrange(len(colors)):
        recohist_array[x].SetLineColor(colors[x])
        mchist_array[x].SetLineColor(kRed)
        graph_array[x].SetLineColor(colors[x])
        mchist_array[x].Draw("same")
        recohist_array[x].Draw("same")
        multigraph.Add(graph_array[x])
        legend.AddEntry(recohist_array[x],"MCTheta: %d" % in_angles[x])

    legend.Draw()
    canvas.cd(2)
    multigraph.Draw("AP")
    legend.Draw()

    canvas.SaveAs(out_dir+"/combined_angles_recotheta_effgraph.png")

def main():
    mykal=KalekoAna()
    #empty containers, get one elem from each input file

    for x in xrange(len(in_fname)):
        mykal.EventLoop(in_path+in_fname[x],int(in_angles[x]),int(max_evts),int(n_bins_histo));

        recohist_array.append(mykal.GetRecoTrackAngleHist())
        mchist_array.append(mykal.GetMCTrackAngleHist())
        graph_array.append(UtilFunctions().GenWindowEfficiencyGraph(mykal.GetRecoTrackAngleHist(),float(mykal.GetMCTrackAngleHist().GetEntries()),mykal.GetMCTrackAngleHist().GetMaximumBin(),100,0.6821))
        nreco_array.append(mykal.GetNRecoTracksHist())


    UtilFunctions().set_style()

    canvas = TCanvas("canvas","My canvas!",100,250,1000,400)
    
    plot_shit();

    print 'Hit enter to exit.'
    sys.stdin.readline()



if __name__ == '__main__':
    main()

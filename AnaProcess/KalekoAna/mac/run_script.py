from ROOT import *
import sys, os
gSystem.Load("libKalekoAna")


max_evts = 1
n_bins_histo = 100


in_path = os.environ.get('MAKE_TOP_DIR')+"/kaleko_input_files_LArLight/"
in_fname = ["v3_ang0_dsout_reduced.root","v3_ang30_dsout_reduced.root","v3_ang45_dsout_reduced.root","v3_ang60_dsout_reduced.root","v3_ang90_dsout_reduced.root"]
out_dir = os.environ.get('MAKE_TOP_DIR')+"/AnaProcess/KalekoAna/mac/output_plots"
in_angles = [ 0, 30, 45, 60, 90 ]
colors = [ kOrange, kBlue, kGreen, kBlack, kMagenta ]
recohist_array = []
mchist_array = []
graph_array = []
nreco_array = []


#multigraph = TMultiGraph("multigraph","Track Reco Efficiency vs. Angle Window around MC Theta")


def plot_effgraphs():

    canvas = TCanvas("canvas","My canvas!",100,250,1000,400)
    canvas.Divide(2)

    legend = TLegend(0.59,0.18,0.83,0.38,"Legend")

    for x in xrange(len(in_angles)):
        canvas.cd(1)

        recohist_array[x].SetLineColor(colors[x])

        mchist_array[x].SetLineColor(kRed)
        mchist_array[x].GetXaxis().SetTitle("Initial ThetaYZ (rad)")

        graph_array[x].SetLineColor(colors[x])
        graph_array[x].SetTitle("Track Reconstruction Efficiency vs Window Size (MCTheta = %d);Window size around MC value (rad);N_Reco_Tracks/N_MC_Tracks" % in_angles[x])
        graph_array[x].SetMaximum(1.1)
        graph_array[x].SetMinimum(0)
        
        legend.Clear()
        legend.SetHeader("Legend")
        legend.AddEntry(recohist_array[x],"3DKalmanSPS Reco")
        legend.AddEntry(mchist_array[x],"MCTheta: %d" % in_angles[x])
        mchist_array[x].Draw()
        recohist_array[x].Draw("same")
        legend.Draw()
        
        canvas.cd(2)

        graph_array[x].Draw("AP*")

        canvas.Modified(); canvas.Update()
        canvas.SaveAs(out_dir+"/angle_%d_recotheta_effgraph.png" % in_angles[x])
        print 'Done with eff/graph for angle %d. Hit enter to continue to next angle.' % in_angles[x]
 #       sys.stdin.readline()

    canvas.Close();
    legend.Delete()
    
def plot_multiplicity():

    canvas2 = TCanvas("canvas2","My second canvas!",100,250,550,400)
    canvas2.SetLogy()

    for x in xrange(len(in_angles)):
        nreco_array[x].GetXaxis().SetTitle("Number Kalman3DSPS Reconstructed Tracks (MCtruth: 1)")
        nreco_array[x].Draw()
        canvas2.Modified(); canvas2.Update()
        canvas2.SaveAs(out_dir+"/angle_%d_multiplicity.png" % in_angles[x])
        print 'Done with multiplicity plot for angle %d. Hit enter to continue to next angle.' % in_angles[x]
#        sys.stdin.readline()

    canvas2.Close()

    

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

    plot_effgraphs()

    plot_multiplicity()
    
    print 'Hit enter to exit.'
    sys.stdin.readline()



if __name__ == '__main__':
    main()

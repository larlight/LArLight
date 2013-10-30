import sys, os
try:
    import ROOT
except AttributeError:
    import ROOT
from ROOT import *
gSystem.Load("libKalekoAna")


max_evts = -1
n_bins_histo = 300


in_path = os.environ.get('MAKE_TOP_DIR')+"/kaleko_input_files_LArLight/"
in_fname = [ "v3_ang0_dsout_reduced.root",
             "v3_ang30_dsout_reduced.root",
             "v3_ang45_dsout_reduced.root",
             "v3_ang60_dsout_reduced.root",
             "v3_ang90_dsout_reduced.root" ]
out_dir = os.environ.get('MAKE_TOP_DIR')+\
          "/AnaProcess/KalekoAna/mac/output_plots"

in_angles = [ 0, 30, 45, 60, 90 ]

recohist_array = []; mchist_array = []; graph_array = []; nreco_array = []


def plot_shit():
    
    f = TFile("histos_out.root","RECREATE")

    canvas = TCanvas("canvas","My canvas!",100,250,1000,400)
    canvas.Divide(2)
    
    pad1 = TPad()
    pad1 = canvas.cd(1)
    pad1.SetLogy()

    #legend in the upper left corner
    legend = TLegend(0.157,0.680,0.398,0.807)

    for x in xrange(len(in_angles)):
        canvas.cd(1)

        recohist_array[x].SetTitle("Kal3DSPS Muon Reco [Initial ThetaYZ - MCTheta]: MCTheta = %d, nMuons = %d;Initial Track ThetaYZ - MCTheta (deg);Counts" % (in_angles[x],mchist_array[x].GetEntries()))
        recohist_array[x].SetLineColor(kBlue)
        recohist_array[x].SetFillColor(kBlue)
        recohist_array[x].SetFillStyle(3002)
        recohist_array[x].SetMaximum(1.1*(mchist_array[x].GetMaximum()))
        recohist_array[x].GetXaxis().CenterTitle()
        recohist_array[x].GetYaxis().CenterTitle()
        
        mchist_array[x].SetLineColor(kRed)
        mchist_array[x].SetFillColor(kRed)
        mchist_array[x].SetFillStyle(3001)
        mchist_array[x].GetXaxis().SetTitle("Initial ThetaYZ (deg)")
        mchist_array[x].GetXaxis().CenterTitle()
        mchist_array[x].GetYaxis().CenterTitle()

        graph_array[x].SetLineColor(kBlue)
        graph_array[x].SetTitle("Track Reconstruction Efficiency vs Window Size (MCTheta = %d, nMuons = %d);Window size around MC value (deg);N_Reco_Tracks/N_MC_Tracks" % (in_angles[x],mchist_array[x].GetEntries()))
        graph_array[x].SetMaximum(1.1)
        graph_array[x].SetMinimum(0)
        graph_array[x].SetFillStyle(3001)
        graph_array[x].SetFillColor(kGray)
        graph_array[x].SetMarkerStyle(kFullDotLarge)
        graph_array[x].GetXaxis().CenterTitle()
        graph_array[x].GetYaxis().CenterTitle()

        legend.Clear()
        legend.AddEntry(recohist_array[x],"3DKalmanSPS Reco")
        legend.AddEntry(mchist_array[x],"MCTheta: %d" % in_angles[x])

        recohist_array[x].Draw()
        mchist_array[x].Draw("same")

        legend.Draw()
        
        canvas.cd(2)
        
        graph_array[x].Draw("A4P")

        canvas.Modified(); canvas.Update()
        canvas.SaveAs(out_dir+"/angle_%d_recotheta_effgraph.png" % in_angles[x])

        mchist_array[x].Write()
        recohist_array[x].Write()
        graph_array[x].Write()

        print 'Done with eff/graph for angle %d.' % in_angles[x]

    
    canvas.Close()
    legend.Delete()
    

    canvas2 = TCanvas("canvas2","My second canvas!",100,250,550,400)
    canvas2.SetLogy()
    
    for x in xrange(len(in_angles)):
        nreco_array[x].GetXaxis().SetTitle("Number Kalman3DSPS Reconstructed Tracks (MCtruth: 1)")
        nreco_array[x].Draw()
        canvas2.Modified(); canvas2.Update()
    
        canvas2.SaveAs(out_dir+"/angle_%d_multiplicity.png" % in_angles[x])

        nreco_array[x].Write()

        print 'Done with multiplicity plot for angle %d.' % in_angles[x]


    canvas2.Close()
    f.Close()


def main():
    mykal=KalekoAna()
    #empty containers, get one elem from each input file

    for x in xrange(len(in_fname)):

        mykal.EventLoop( in_path+in_fname[x],
                        int(in_angles[x]),
                        int(max_evts),
                        int(n_bins_histo) );

        recohist_array.append(mykal.GetRecoTrackAngleHist())
        mchist_array.append(mykal.GetMCTrackAngleHist())
        nreco_array.append(mykal.GetNRecoTracksHist())

        graph_array.append(UtilFunctions().GenWindowEfficiencyGraph(mykal.GetRecoTrackAngleHist(),float(mykal.GetMCTrackAngleHist().GetEntries()),mykal.GetMCTrackAngleHist().GetMaximumBin(),20,0.6821))


    UtilFunctions().set_style()

    plot_shit()
    
    print 'Hit enter to exit.'
    sys.stdin.readline()



if __name__ == '__main__':
    main()

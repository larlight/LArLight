import sys, os
try:
    import ROOT
except AttributeError:
    import ROOT
from ROOT import *
gSystem.Load("libKalekoAna")


reco_type = "Kalman3DSPS" # "Bezier" or "Kalman3DSPS"

max_evts = 3000
n_bins_histo = 100


#in_path = os.environ.get('MAKE_TOP_DIR')+"/kaleko_input_files_LArLight/"
in_path = "/a/data/morningside/kaleko"+"/kaleko_input_files_LArLight/"
#in_fname = [ "v4_ang0_dsout_reduced_%s.root" % reco_type,
#             "v4_ang10_dsout_reduced_%s.root" % reco_type,
#             "v4_ang20_dsout_reduced_%s.root" % reco_type,
#             "v4_ang30_dsout_reduced_%s.root" % reco_type,
#             "v4_ang40_dsout_reduced_%s.root" % reco_type,
#             "v4_ang50_dsout_reduced_%s.root" % reco_type,
#             "v4_ang60_dsout_reduced_%s.root" % reco_type,
#             "v4_ang70_dsout_reduced_%s.root" % reco_type,
#             "v4_ang80_dsout_reduced_%s.root" % reco_type,
#             "v4_ang90_dsout_reduced_%s.root" % reco_type ]
in_fname = [ "v4_ang90_dsout_reduced_Kalman3DSPS.root" ]

out_dir = os.environ.get('MAKE_TOP_DIR')+\
          "/AnaProcess/KalekoAna/mac/output_plots/v4_%s" % reco_type

#in_angles = [ 0, 10, 20, 30, 40, 50, 60, 70, 80, 90 ]
in_angles = [ 90 ]

recohist_array = []; mchist_array = []; graph_array = []; nreco_array = []


def plot_shit():
    
    f = TFile("%s/histos_v4_%s.root" % (out_dir,reco_type),"RECREATE")

    canvas = TCanvas("canvas","My canvas!",100,250,1000,400)
    canvas.Divide(2)
    
    pad1 = TPad()
    pad1 = canvas.cd(1)
    pad1.SetLogy()

    #legend in the upper left corner
    legend = TLegend(0.157,0.680,0.398,0.807)

    for x in xrange(len(in_angles)):
        canvas.cd(1)

        recohist_array[x].SetTitle("%s Muon Reco [Initial ThetaYZ - MCTheta]: MCTheta = %d, nMuons = %d;Initial Track ThetaYZ - MCTheta (deg);Counts" % (reco_type,in_angles[x],max_evts))
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
        graph_array[x].SetTitle("Track Reconstruction Efficiency vs Window Size (MCTheta = %d, nMuons = %d);Window size around MC value (deg);N_Reco_Tracks/N_MC_Tracks" % (in_angles[x],max_evts))
        graph_array[x].SetMaximum(1.1)
        graph_array[x].SetMinimum(0)
        graph_array[x].SetFillStyle(3001)
        graph_array[x].SetFillColor(kGray)
        graph_array[x].SetMarkerStyle(kFullDotLarge)
        graph_array[x].GetXaxis().CenterTitle()
        graph_array[x].GetYaxis().CenterTitle()

        legend.Clear()
        legend.AddEntry(recohist_array[x],"%s Reco" % reco_type)
        legend.AddEntry(mchist_array[x],"MCTheta: %d" % in_angles[x])

        recohist_array[x].Draw()
        mchist_array[x].Draw("same")

        legend.Draw()
        
        canvas.cd(2)
        
        graph_array[x].Draw("A4P")

        canvas.Modified(); canvas.Update()
        canvas.SaveAs(out_dir+"/angle_%d_recotheta_effgraph_%s.png" % (in_angles[x],reco_type))

        mchist_array[x].Write()
        recohist_array[x].Write()
        graph_array[x].Write()

        print 'Done with eff/graph for angle %d.' % in_angles[x]

    
    canvas.Close()
    legend.Delete()
    

    canvas2 = TCanvas("canvas2","My second canvas!",100,250,550,400)
    canvas2.SetLogy()
    
    for x in xrange(len(in_angles)):
        nreco_array[x].GetXaxis().SetTitle("Number %s Reconstructed Tracks (MCtruth: 1)" % reco_type)
        nreco_array[x].Draw()
        canvas2.Modified(); canvas2.Update()
    
        canvas2.SaveAs(out_dir+"/angle_%d_multiplicity_%s.png" % (in_angles[x],reco_type))

        nreco_array[x].Write()

        print 'Done with multiplicity plot for angle %d.' % in_angles[x]


    canvas2.Close()
    f.Close()


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

        recohist_array.append(mykal.GetRecoTrackAngleHist())
        mchist_array.append(mykal.GetMCTrackBoxAngleHist())
        nreco_array.append(mykal.GetNRecoTracksHist())

        graph_array.append(UtilFunctions().GenWindowEfficiencyGraph(mykal.GetRecoTrackAngleHist(),float(mykal.GetMCTrackAngleHist().GetEntries()),mykal.GetMCTrackAngleHist().GetMaximumBin(),0.6821))


    UtilFunctions().set_style()

    plot_shit()
    
    print 'Hit enter to exit.'
    sys.stdin.readline()



if __name__ == '__main__':
    main()

#
# Example PyROOT script to run analysis module, ana_base.
# The usage is same for inherited analysis class instance.
#

# Load libraries
import os, ROOT, sys
from ROOT import gSystem
gSystem.Load("libAnalysis")
gSystem.Load("libKalekoAna")

# Now import ana_processor & your class. For this example, ana_base.
from ROOT import *

# Create ana_processor instance
my_proc=ana_processor()

# Specify IO mode
my_proc.set_io_mode(storage_manager.READ)
#my_proc.set_io_mode(storage_manager.WRITE)
#my_proc.set_io_mode(storage_manager.BOTH)

# Set input root file: this is decoder output root file.
# This time, we use a sample file prepared.
my_proc.add_input_file(sys.argv[1])

storage_manager.get().set_in_rootdir("scanner")

# Set output root file: this is a separate root file in which your
# analysis module can store anything such as histograms, your own TTree, etc.
my_proc.set_ana_output_file("myout.root")

# Add analysis unit to process
my_ana = TrackViewer()
my_proc.add_process(my_ana)

# Set TDirectory name in the ROOT file where TTrees can be found
# If no TDirectory, comment out.

ctr=0
c=TCanvas("c","",600,500)

while my_proc.process_event():

    hMC   = my_ana.GetHisto_MC()
    hSPS  = my_ana.GetHisto_SPS()
    vReco = my_ana.GetHisto_Reco()
    #vData is a vector of <track> objects
    vData = my_ana.GetData_Reco()


    hMC_xy = hMC.Project3D("xy")
    hSPS_xy = hSPS.Project3D("xy")
    

    first_draw=True

    for x in xrange(vData.size()):
        #if you want to look at angle b/t first two reco track points....
        reco_theta = vData.at(x).direction_at(0).Theta() * UtilFunctions().DegreesPerRadian;
        #if you want to look at angle b/t first and last reco track points...
        #reco_theta = (vData.at(x).direction_at(vData.at(x).n_point()-1) - vData.at(x).direction_at(0)).Theta()*UtilFunctions().DegreesPerRadian;
        #if you want to look at a weighted average angle (weighted by distance b/t points)...
        #reco_theta = UtilFunctions().CalculateWeightedAvgTheta(vData.at(x)) * UtilFunctions().DegreesPerRadian;
        
        print "processing ctr_evt %d. reco_theta = %f. # tracks in evt = %d" % (ctr,reco_theta,vData.size())
        
        if reco_theta < 95 and reco_theta > 85:
            print "reco_theta is bt 85 and 95"
        
            if hSPS_xy:
                hSPS_xy.Draw("BOX")
                first_draw=False
            
            if hMC_xy:
                if first_draw: 
                    hMC_xy.Draw("BOX")
                else:
                    hMC_xy.Draw("BOX sames")
                
            print
            print "Entry %d ... found %d spacepoints & %d reco-ed track" % (ctr,hSPS.GetEntries(),vReco.size())
                        
            for x in xrange(vReco.size()):
                print "Plotting track %d " % vData[x].track_id()
                print "Number of points: %d" % vData[x].n_point()
                vReco[x].Project3D("xy").Draw("BOX sames")
                c.Update()

            sys.stdin.readline()


    print "continuing to next evt..."
    ctr+=1
# done!

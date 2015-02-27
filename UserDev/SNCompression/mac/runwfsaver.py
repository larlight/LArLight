import ROOT, sys, os
from ROOT import *
from matplotlib import pyplot as plt
import numpy as np
import scipy

gSystem.Load('libAnalysis')
gSystem.Load('libLArUtil')
gSystem.Load('libSNCompression')

filename = sys.argv[1]

#open root file and go to scanner directory
f = ROOT.TFile(filename,"READ")
d = f.Get("scanner")

tpcTree = d.Get("tpcfifo_tree")
ev_tpc = larlight.tpcfifo()
for event in tpcTree:
    
    ev_tpc = tpcTree.tpcfifo_branch
    
    image = np.zeros((9600,3000))
    image.fill(400)
    counter = 0
    try:
        counter = input('Hit Enter to continue to next evt, or type in an event number to jump to that event:')
    except SyntaxError:
        counter = counter + 1
    for w in ev_tpc:
        if (w.plane() == 2):
            time = w.readout_sample_number_RAW()+1
            for tick in xrange(w.size()):
                image[tick+time,w.channel_number()-4976] = float(w[tick])#-2040.
    image = (image-image.min())/(np.ptp(image))
    print image.min()
    scipy.misc.imsave('out.jpg',image)
    #for x in xrange(1000):
    #        for y in xrange(1000):
    #            if (image[x,y] != 0):
    #                print image[x,y]
    #implot = plt.imshow(image)
    #implot 
    #plt.colorbar(orientation='vertical')
    #plt.show()
    #print "hello"
        #print w.plane()
#tpcTree.SetBranchAddress("tpcfifo_branch",ev_tpc)
#tpcTree.GetEntry(1)
#print ev_tpc[0].plane()

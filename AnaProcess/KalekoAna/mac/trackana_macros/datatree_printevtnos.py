import ROOT
from ROOT import *

f = TFile("DataTree_v4_ang60.root","READ")
tree = f.Get("tree")

for x in xrange(tree.GetEntries()):
    tree.GetEntry(x)
    for y in xrange(tree.myRecoTrackAngle.size()):
        if tree.myRecoTrackAngle[y] > 85 and tree.myRecoTrackAngle[y] < 95:
            print "evtno = %d, angle is %f" % (tree.myEventNumber,tree.myRecoTrackAngle[y])

    

f.Close()

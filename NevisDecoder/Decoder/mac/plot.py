import sys
from ROOT import *
gSystem.Load("libDataFormat")

ch=TChain("tpcfifo_tree")
ch.AddFile(sys.argv[1])

if not ch.GetEntries():
    print "No event found!"
    sys.exit(1)
c=TCanvas("c","",600,500)
for x in xrange(ch.GetEntries()):

    ch.GetEntry(x)

    wfs = ch.tpcfifo_branch
    print "Event %d ... %d channels found!" % (wfs.event_id(), wfs.size())

    for y in xrange(wfs.size()):

        wf = wfs.at(y)

        print "  Channel: %d" % wf.channel_number()

        h=TH1D("hWF","Event %d Channel %d; Time Tick; ADC" % (wfs.event_id(),wf.channel_number()),
               wf.size(),-0.5,wf.size()-0.5)
        for z in xrange(wf.size()):
            h.SetBinContent(z+1,wf.at(z))

        c.cd()
        h.Draw()
        c.Update()
        sys.stdin.readline()

        del h
    print
    print "done with event %d..." % wfs.event_id()
    print

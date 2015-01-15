from ROOT import TChain,TH1D,TCanvas
import sys
chain=TChain("tpcfifo_tree")
chain.AddFile(sys.argv[1])

chain.GetEntries()
c=TCanvas('c','',600,500)
c.SetGridx(1)
c.SetGridy(1)
for x in xrange(chain.GetEntries()):
    chain.GetEntry(x)
    wf_v=chain.tpcfifo_branch

    for y in xrange(wf_v.size()):

        wf = wf_v[y]
        print wf
        ch = wf.channel_number()
        print 'Channel:',ch
        print wf
        print wf.size()
        h=TH1D("hWF","Event %d Waveform for Channel %d; Ticks; ADC" % (wf_v.event_number(),ch),
               wf.size(),-0.5,wf.size()-0.5)

        for z in xrange(wf.size()):
            h.SetBinContent(z+1,wf[z])

        h.Draw()
        c.Update()
        sys.stdin.readline()
        del h
        

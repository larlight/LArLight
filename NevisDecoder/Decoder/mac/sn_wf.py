from ROOT import TChain,TH1D,TCanvas
import sys
chain=TChain("tpcfifo_tree")
chain.AddFile(sys.argv[1])

chain.GetEntries()
c=TCanvas('c','',600,500)
c.SetGridx(1)
c.SetGridy(1)
for x in xrange( chain.GetEntries() ):
    chain.GetEntry(x)
    wfs = chain.tpcfifo_branch

    for y in xrange( wfs.size() ):

        wf = wfs[y]
        print wf
        ch = wf.channel_number()
        print 'Channel: ', ch
        print wf
        print 'Waveform size: ', wf.size()
        time = wf.readout_sample_number_RAW()
        print 'Time: ', time
        h = TH1D("hWF","Event %d Waveform for Channel %d Time %d; Ticks; ADC" 
               % ( wfs.event_number(), ch, time ),
               wf.size(), time-0.5 , time+wf.size()+0.5 )

        for z in xrange( wf.size() ):
            print "tick: %d, ADC count: %d" %( z, wf[z] )
            h.SetBinContent( z+1, wf[z] )

        h.SetLineWidth( 3 )
        h.SetLineColor( 593 )
        h.Draw()
        c.Update()
        epsname = "WF_Evt%d_Ch%d_Time%d.eps" %( wfs.event_number(), ch, time)
        c.SaveAs( epsname )
        sys.stdin.readline()
        del h
        

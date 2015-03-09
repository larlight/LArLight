from ROOT import TChain,TH1D,TCanvas
import sys
fakefile = None
chain = TChain("tpcfifo_tree")

for infile in sys.argv[1:]:
    if ".txt" in infile:
        fakefile = infile
    else:
        chain.AddFile( infile )

# Read in the fake data pattern
fdata = []
if fakefile:
    f = open( fakefile, "r" )
    for line in f:
        fdata += line.strip().split()

    print "len(fdata): ", len(fdata)
    toffset = 2

# Output canvas
c = TCanvas('c','',600,500)
c.SetGridx(1)
c.SetGridy(1)

chain.GetEntries()

for x in xrange( chain.GetEntries() ):
    chain.GetEntry(x)
    wfs = chain.tpcfifo_branch
    ie = wfs.event_number()
    print 'Event: ', ie

    for y in xrange( wfs.size() ):

        wf = wfs[y]
        print wf
        ch = wf.channel_number()
        print 'Channel: ', ch
        print 'Waveform size: ', wf.size()
        time = wf.readout_sample_number_RAW()
        if wf.size() == 0:
            time = 0
        print 'Time: ', time

        faketime = ( time - (ie%2 + 1) * (len(fdata)/2) ) % len(fdata)
        print "Fake Time: ", faketime

        h = TH1D("hWF", "Event %d Waveform for Channel %d Time %d; Ticks; ADC" 
                 % ( ie, ch, time ),
                 wf.size(), time-0.5 , time+wf.size()+0.5 )

        g = TH1D("hFake", "Event %d Waveform for Channel %d Time %d; Ticks; ADC"
                 % ( ie, ch, time ),
                 wf.size(), time-0.5 , time+wf.size()+0.5 )

        for z in xrange( wf.size() ):
            print "tick: %d, ADC count: %d" %( z, wf[z] )
            h.SetBinContent( z+1, wf[z] )
            if fdata:
                g.SetBinContent( z+1, int(fdata[faketime-1-toffset+z]) )

        h.SetLineWidth( 3 )
        h.SetLineColor( 593 )
        h.Draw( "AXIS" )

        if fdata:
            g.SetLineWidth( 5 )
            g.SetLineColor( 906 )
            g.Draw( "same" )

        h.Draw( "same" )
        c.Update()
        epsname = "WF_Evt%d_Ch%d_Time%d.eps" %( wfs.event_number(), ch, time)
        c.SaveAs( epsname )
        sys.stdin.readline()
        del h, g
        

from ROOT import *
import sys

ch = TChain("pmtfifo_tree")
ch.AddFile(sys.argv[1])

if not ch.GetEntries():
    sys.exit(1)

c = TCanvas("c","name",600,500)
c5 = TCanvas("c5","name5",600,500)
c6 = TCanvas("c6","name6",600,500)

htmp5 = TH1D("htmp5","histtitle",100,-0.5,99.5)
htmp6 = TH1D("htmp6","histtitle",100,-0.5,99.5)

for evt in xrange(ch.GetEntries()):
    ch.GetEntry(evt)

    wfs = ch.pmtfifo_branch
    print "event number = ",wfs.event_number()

    #if wfs.event_number() < 35: continue
    
    for pmt in xrange(wfs.size()):

        if wfs[pmt].channel_number() not in [5,6]: 
            continue        

        if wfs[pmt].channel_number() == 6 and wfs[pmt].readout_sample_number_RAW() < 100:
            htmp6.SetBinContent(wfs[pmt].readout_sample_number_RAW(),wfs[pmt].size())
        if wfs[pmt].channel_number() == 5 and wfs[pmt].readout_sample_number_RAW() < 100:
            htmp5.SetBinContent(wfs[pmt].readout_sample_number_RAW(),wfs[pmt].size())
#        continue

        print "channel_number        = ", wfs[pmt].channel_number()
        print "discrim_id            = ", wfs[pmt].disc_id()
        print "readout_frame_number  = ", wfs[pmt].readout_frame_number()
        #this is the sample number within this frame that readout starts
        print "readout_sample_number = ", wfs[pmt].readout_sample_number_RAW()
        print "reading out %d => %d" % \
            (wfs[pmt].readout_sample_number_RAW(),\
             wfs[pmt].readout_sample_number_RAW() + wfs[pmt].size())

        h = TH1D("h","myhist",wfs[pmt].size(),
                 wfs[pmt].readout_sample_number_RAW()-0.5,
                 wfs[pmt].readout_sample_number_RAW()+wfs[pmt].size()-0.5)

        for timetick in xrange(wfs[pmt].size()):
            h.SetBinContent(timetick+1,wfs[pmt][timetick])

        c.cd()
        h.Draw()
        c.Update()

        sys.stdin.readline()
        del h
        

c5.cd()
htmp5.Draw()
c5.Update()
c6.cd()
htmp6.Draw()
c6.Update()

sys.stdin.readline()


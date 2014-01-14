import os,sys
import signal

import ROOT
ROOT.gSystem.Load("libdecoder")
from ROOT import decoder, FORMAT, MSG, storage_manager, TH1D, TCanvas

# 
# Decode
#
tool=decoder()
tool.set_format(FORMAT.ASCII)
tool.set_filename("test_pmt.dat")
tool.debug_mode(True)
tool.run();

#
# Analyze
#
storage=storage_manager.get()
storage.set_filename("out.root")
storage.set_io_mode(storage_manager.READ)
storage.open()

c=TCanvas("c","c",800,600)
h=0

#Loop over recorded events
while storage.next_event():
    hArray=[] # histogram array

    # get event-wise data ... event_waveform class instance
    event=storage.get_data()

    # loop over channels to search for max # of time slice
    max_length=0
    for x in xrange(event.size()):
        if event[x].size() > max_length:
            max_length=event[x].size()

    # make a histogram
    h=0
    height_max=0
    height_min=0
    for x in xrange(event.size()):
        # event[x] is one PMT waveform object (pmt_waveform class instance)
        h=TH1D("hWF_Ch%03d" % event[x].channel_number(),
               "Ch. %-3d Waveform; Time slice; ADC" % event[x].channel_number(), 
               max_length,0,float(max_length))
        for y in xrange(max_length):
            adc=2000.
            if y < event[x].size():
                adc=float(event[x][y])
            h.SetBinContent(y+1,adc)
            if height_max < adc:
                height_max=adc
            if height_min > adc:
                height_min=adc
        hArray.append(h)
        
    for h in hArray:
        h.SetMaximum(height_max+100)
        h.SetMinimum(height_min-100)

        if h==hArray[0]:
            h.Draw()
        else:
            h.Draw("sames")

    sys.stdout.write('\nType anything to move to next event.')
    sys.stdout.flush()
    user_input=sys.stdin.readline()

    for h in hArray:
        print "removing:",h.GetName()
        del h
        h=0
    del hArray

storage.close()


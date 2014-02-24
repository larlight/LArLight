import sys
import ROOT
ROOT.gSystem.Load("libTriggerSim")
from ROOT import trigger, trigdata, std

# Create trigger algorithm instance
trig = trigger.UBTriggerAlgo()

# Configure
trig.SetDebugMode(True)
trig.SetMask(8,0xfd)

# Report configuration & clear stored trigger candidates (should be empty)
trig.ReportConfig()
trig.ClearInputTriggers()

# Add some PMT-cosmic trigger ... unit is in second
trig.AddTriggerPMTCosmic(0.)

# Add some calibration trigger ... unit is in second
trig.AddTriggerCalib(0.0032 + 180.e-6)

# Add some external trigger ... unit is in second
trig.AddTriggerExt(0.0032 + 120.e-6)

# This will be picked up
trig.AddTriggerBNB(0.0016)

# This will be skipped as it crosses the deadtime boundary
trig.AddTriggerNuMI(0.0016 + 0.0064 - 1.e-9)

# This will be picked up
trig.AddTriggerNuMI(0.0016 + 0.0064 + 1.e-9)

aho=std.vector(trigdata.Trigger)()

trig.ProcessTrigger(aho)

print
print "Done processing Trigger. We found following triggers..."
print
for x in xrange(aho.size()):

    print "Found Trigger @ (%d, %d) with bits %d" % (aho.at(x).TriggerSample(),
                                                     aho.at(x).TriggerFrame(),
                                                     aho.at(x).TriggerBits())

print
print "Done listing found triggers."
print

import sys
import ROOT
#ROOT.gSystem.Load("libTriggerSim")
ROOT.gSystem.Load("libTimeService")
from ROOT import trigger, raw, std, util

def GetClock(time) :
    return util.TimeService.GetME().OpticalClock(time)

# Create trigger algorithm instance
trig = trigger.UBTriggerAlgo()

# Configure
trig.SetDebugMode(True)
trig.SetMask(8,0xfd)

# Report configuration & clear stored trigger candidates (should be empty)
trig.ReportConfig()
trig.ClearInputTriggers()

# Add some PMT-cosmic trigger ... unit is in nano-second
clock = GetClock(0)
trig.AddTriggerPMTCosmic(clock)

# Add some calibration trigger ... unit is in nano-second
clock = GetClock(3.2e6 + 180.e3)
trig.AddTriggerCalib(clock)

# Add some external trigger ... unit is in second
clock = GetClock(3.2e6 + 120.e3)
trig.AddTriggerExt(clock)

# This will be picked up
clock = GetClock(1.6e6)
trig.AddTriggerBNB(clock)

# This will be skipped as it crosses the deadtime boundary
clock = GetClock(1.6e6 + 6.4e6 - 1)
trig.AddTriggerNuMI(clock)

# This will be picked up
clock = GetClock(1.6e6 + 6.4e6 + 1)
trig.AddTriggerNuMI(clock)

aho=std.vector(raw.Trigger)()

trig.ProcessTrigger(aho)

print
print "Done processing Trigger. We found following triggers..."
print
for x in xrange(aho.size()):

    clock = util.TimeService.GetME().TriggerClock(aho.at(x).TriggerTime())
    print "Found Trigger @ (%d, %d) with bits %d" % (clock.Sample(), clock.Frame(), aho.at(x).TriggerBits())

print
print "Done listing found triggers."
print

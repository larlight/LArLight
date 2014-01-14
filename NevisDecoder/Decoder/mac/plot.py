# Import basic python module
import sys

# Place input argument in a temporary var. holder (else ROOT misinterprets)
argv=sys.argv
sys.argv=[]

# ROOT import
from ROOT import gSystem, TCanvas

# Decoder analysis library import
gSystem.Load("libAnalysis")

# Analysis library/class import
from ROOT import ana_processor, reco_wf, MSG, storage_manager

# processor instance creation
proc=ana_processor()

# I/O mode ... READ, WRITE, or BOTH
proc.set_io_mode(storage_manager.READ)

# input file specification
proc.add_input_file(argv[1])

# output file specification (will be empty in this script)
proc.set_ana_output_file("ana_out.root")

# analysis module creation. "reco_wf" is an analysis module
# that reconstructs the waveform by interpretting frame, timeslice,
# and adc sample information. "reco_wf" is a singleton class,
# so you call it's static "get" member function to obtain its instance.
ana=reco_wf()
ana.set_frame_width(1024) # tune to the value you know

# Attach this analysis module to the processor.
# You can add arbitrary number of analysis modules in running order.
proc.add_process(ana);

# Make a canvas
c=TCanvas("c","",800,600)

# Make a histogram pointer holder
h=0

# Loop in events ... this is an interactive running asking a user 
# prompt in each loop step. In case of batch running, simply call 
# ana_processor::run() member function.
while 1:

    # Process one event
    proc.process_event()

    if not proc.get_process_status() == proc.PROCESSING:
        break

    # Get a reco-ed wf histogram ... this histogram is created only
    # upon this function call as it is possibly a memory intensive
    # object. A recommended access is to copy a pointer of stl
    # vector waveform container. See reco_wf() class reference.
    h=ana.get_histo();
    if not h:
        continue

    h.SetMaximum(4096)
    h.SetMinimum(0)

    h.Draw("LEGO2");
    c.Modified()
    c.Update()
    c.Draw()
    print
    print "Type anything to move onto the next event."
    user_input=sys.stdin.readline()

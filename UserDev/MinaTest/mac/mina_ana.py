import sys
from ROOT import *

ch=TChain("myana/toy_tree","")

ch.AddFile(sys.argv[1])

print "Found",ch.GetEntries(),"entries!"

for x in xrange(ch.GetEntries()):

    print "TChain entry:",x
    ch.GetEntry(x)

    my_toy_event = ch.toy_event_branch

    print "  Found %d particles!" % my_toy_event.size()
    print "  Listing PDG ID of particles...",
    for y in xrange(my_toy_event.size()):

        print my_toy_event.at(y).pdg_id(),

    print
    print "done with entry",x
    print

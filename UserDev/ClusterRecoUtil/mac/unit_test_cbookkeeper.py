import ROOT
ROOT.gSystem.Load("libClusterRecoUtil")
from ROOT import *

bk = cluster.CBookKeeper()

print
print " ****** Running a unit testing for CBookKeeper ****** "
print

NENTRIES=10
COMBINE_SET_1 = [0,1,2]
COMBINE_SET_2 = [3,4,5]
COMBINE_SET_3 = [6,7,8]
print " # entries = %d" % NENTRIES
bk.Reset(NENTRIES)
#result = bk.GetResult()
#print " Current result (%d entries)..." % result.size()
#for x in xrange(result.size()):
#    indexes = result.at(x)
#    print " Group %d:" % x,
#    for y in xrange(indexes.size()):
#        print indexes.at(y),
#    print

print
print " Combining:",COMBINE_SET_1
for x in xrange(len(COMBINE_SET_1)):
    if x == 0: continue
    bk.Merge(COMBINE_SET_1[0],COMBINE_SET_1[x])
result = bk.GetResult()
print " Current result (%d entries)..." % result.size()
for x in xrange(result.size()):
    indexes = result.at(x)
    print " Group %d:" % x,
    for y in xrange(indexes.size()):
        print indexes.at(y),
    print

print
print " Combining:",COMBINE_SET_2
for x in xrange(len(COMBINE_SET_2)):
    if x == 0: continue
    bk.Merge(COMBINE_SET_2[0],COMBINE_SET_2[x])
result = bk.GetResult()
print " Current result (%d entries)..." % result.size()
for x in xrange(result.size()):
    indexes = result.at(x)
    print " Group %d:" % x,
    for y in xrange(indexes.size()):
        print indexes.at(y),
    print

print
print " Combining:",COMBINE_SET_3
for x in xrange(len(COMBINE_SET_3)):
    if x == 0: continue
    bk.Merge(COMBINE_SET_3[0],COMBINE_SET_3[x])
result = bk.GetResult()
print " Current result (%d entries)..." % result.size()
for x in xrange(result.size()):
    indexes = result.at(x)
    print " Group %d:" % x,
    for y in xrange(indexes.size()):
        print indexes.at(y),
    print

if len(COMBINE_SET_1) and len(COMBINE_SET_2):

    COMBINE_SET_4 = [COMBINE_SET_1[0],COMBINE_SET_2[0]]
    print
    print " Combining:",COMBINE_SET_4
    bk.Merge(COMBINE_SET_4[0],COMBINE_SET_4[1])
    result = bk.GetResult()
    print " Current result (%d entries)..." % result.size()
    for x in xrange(result.size()):
        indexes = result.at(x)
        print " Group %d:" % x,
        for y in xrange(indexes.size()):
            print indexes.at(y),
        print

# Now let's make another book keeper that acts on the result on the 1st one.
result = bk.GetResult()
if result.size() > 1:

    abk = cluster.CBookKeeper(result.size())
    print
    print " Creating another BookKeeper on the result (length=%d)" % result.size()

    COMBINE_SET_5 = [0,1]
    print " Combining:",COMBINE_SET_5
    abk.Merge(COMBINE_SET_5[0],COMBINE_SET_5[1])
    aresult = abk.GetResult()
    print " New instance's result (%d entries)..." % aresult.size()
    for x in xrange(aresult.size()):
        indexes = aresult.at(x)
        print " Group %d:" % x,
        for y in xrange(indexes.size()):
            print indexes.at(y),
        print

    print
    print " Combining two CBookKeeper instances..."
    
    bk.Combine(abk)
    result = bk.GetResult()
    print " Current result (%d entries)..." % result.size()
    for x in xrange(result.size()):
        indexes = result.at(x)
        print " Group %d:" % x,
        for y in xrange(indexes.size()):
            print indexes.at(y),
        print




print



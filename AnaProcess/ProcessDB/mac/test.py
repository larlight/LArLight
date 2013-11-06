from ROOT import gSystem
gSystem.Load("libProcessDB")
from ROOT import *
k=ProcessDBI()
passwd=open("/grid/fermiapp/lbne/lar/aux/.pswd","r").read().rstrip("\n")
k.Connect("fnalpgsdev.fnal.gov",
          5436,
          "uboonedaq_dev",
          "uboonedaq_reader",
          passwd)
key=k.Query("SELECT table_name FROM information_schema.tables WHERE table_schema='public' AND table_name='crates'")
print k.GetRowCount(key)
res=k.FetchRow(key)
ncolumn = k.GetFieldCount(key)
while res:
    for x in xrange(ncolumn):
        print res.GetString(x),
    print
    res=k.FetchRow(key)
k.ClearResult(key)
k.Disconnect()


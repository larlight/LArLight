from ROOT import gSystem
gSystem.Load("libProcessDB")
from ROOT import *
k=ProcessDBI(ProcessDBI.MYSQL)
k.Connect("ccdchoozdb.in2p3.fr",
          0,
          "dc_file",
          "dcreader",
          "DCfarPub!")
res=k.Query("Show Tables")
print k.GetRowCount(res)
k.ClearResult(res)
k.Disconnect()

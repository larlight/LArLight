from ROOT import gSystem
gSystem.Load("libProcessDB")
from ROOT import *
k=TableBase("crates")
passwd=open("/grid/fermiapp/lbne/lar/aux/.pswd","r").read().rstrip("\n")
k.Connect("fnalpgsdev.fnal.gov",
          5436,
          "uboonedaq_dev",
          "uboonedaq_reader",
          passwd)
print k.ExistTable();
k.QueryAndDump("SELECT COLUMN_NAME, DATA_TYPE, CHARACTER_MAXIMUM_LENGTH FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = 'crates';");
k.QueryAndDump("SELECT * FROM crates;")
ProcessDBIManager.GetME().CloseAllConnection();



from ROOT import gSystem, std
gSystem.Load("libUBOnlineDBI")
from ROOT import ubpsql as db

# First set CUSTOM_USER config string for UBDAQ_WRITER
reader_dbi = db.DBConn.GetME(db.kUBDAQ_READER)
custom_dbi = db.DBConn.GetME(db.kCUSTOM_USER)
reader_dbi.Configure(custom_dbi.GetConnectionInfo())

api = db.RCReader()

api.Connect()

api.GetRunConfigIDList()

api.GetSubConfigIDList(0)

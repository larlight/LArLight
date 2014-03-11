from ROOT import gSystem, std
gSystem.Load("libUBOnlineDBI")
from ROOT import ubpsql as db

# First set CUSTOM_USER config string for UBDAQ_WRITER
writer_dbi = db.DBConn.GetME(db.kUBDAQ_WRITER)
custom_dbi = db.DBConn.GetME(db.kCUSTOM_USER)
writer_dbi.Configure(custom_dbi.GetConnectionInfo())

# Set to debug mode
writer_dbi.SetVerbosity(db.

# Get a writer instance
writer_api = db.RCWriter()

# Create hstore_column to be recorded
hstore_column = std.map(std.string,std.string)()
hstore_column['par0']='var0'
hstore_column['par1']='var1'



writer_api.InsertSubConfigValuesPerCrate("ktest1",1,0,hstore_column)

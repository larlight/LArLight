from ROOT import gSystem, std
gSystem.Load("libUBOnlineDBI")
from ROOT import ubpsql as db

# First set CUSTOM_USER config string for UBDAQ_WRITER
writer_dbi = db.DBConn.GetME(db.kUBDAQ_WRITER)
custom_dbi = db.DBConn.GetME(db.kCUSTOM_USER)
writer_dbi.Configure(custom_dbi.GetConnectionInfo())

# Set to debug mode
writer_dbi.SetVerbosity(db.MSG.kDEBUG)

# Get a writer instance
writer_api = db.RCWriter()

# Create hstore_column to be recorded
hstore_column = std.vector(std.string)()
hstore_column.push_back('par0')
hstore_column.push_back('par1')
hstore_column.push_back('nchannels')

writer_api.CreateConfigType("ktest3",hstore_column)


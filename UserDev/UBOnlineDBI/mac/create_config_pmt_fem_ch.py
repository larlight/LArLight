from ROOT import gSystem, std
gSystem.Load("libUBOnlineDBI")
from ROOT import ubpsql as db

# First set CUSTOM_USER config string for UBDAQ_WRITER
writer_dbi = db.DBConn.GetME(db.kUBDAQ_WRITER)
custom_dbi = db.DBConn.GetME(db.kCUSTOM_USER)
writer_dbi.Configure(custom_dbi.GetConnectionInfo())

# Set to debug mode
#writer_dbi.SetVerbosity(db.MSG.kDEBUG)

# Get a writer instance
writer_api = db.RCWriter()

# Create hstore_column to be recorded
hstore_column = std.vector(std.string)()
hstore_column.push_back('deadtime')
hstore_column.push_back('disc_width')
hstore_column.push_back('disc_precount')
hstore_column.push_back('disc_threshold0')
hstore_column.push_back('disc_threshold1')
hstore_column.push_back('disc_threshold3')
hstore_column.push_back('pmt_words_cosmic')
hstore_column.push_back('michel_window_width')

writer_api.CreateConfigType("pmt_fem_ch",hstore_column)


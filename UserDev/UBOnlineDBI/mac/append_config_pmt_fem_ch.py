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
hstore_column = std.map(std.string,std.string)()
hstore_column['deadtime']         = '25'
hstore_column['disc_width']       = '5'
hstore_column['disc_precount']    = '3'
hstore_column['disc_threshold0']  = '3'
hstore_column['disc_threshold1']  = '5'
hstore_column['disc_threshold3']  = '5'
hstore_column['pmt_words_cosmic'] = '20'
hstore_column['michel_window_width'] = '0' 

# PMT FEM MODULE config ... crate = crate ID
writer_api.InsertSubConfigValues("pmt_fem_ch", # config name
                                 0,             # config ID
                                 0,             # crate   = means nothing (always 0)
                                 0,             # channel = means nothing (always 0)
                                 hstore_column  # parameter sets
                                 )



import ROOT, sys
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

# Variable holders
config_name   = ""
config_id     = -1
crate_id      = -1
hstore_column = std.map(std.string,std.string)()

# Trigger configuration
config_name   = 'pmtconfig'
config_id     = 0
crate_id      = 0
hstore_column.clear()
hstore_column['module_address'] = '1'
hstore_column['frame_size'    ] = '25600'
hstore_column['mask0'         ] = '0x0'
hstore_column['mask1'         ] = '0x0'
hstore_column['mask2'         ] = '0x0'
hstore_column['mask3'         ] = '0x0'
hstore_column['mask4'         ] = '0x0'
hstore_column['mask5'         ] = '0x0'
hstore_column['mask6'         ] = '0x0'
hstore_column['mask7'         ] = '0x0'
hstore_column['mask8'         ] = '0x0'
hstore_column['prescale0'     ] = '0x0'
hstore_column['prescale1'     ] = '0x0'
hstore_column['prescale2'     ] = '0x0'
hstore_column['prescale3'     ] = '0x0'
hstore_column['prescale4'     ] = '0x0'
hstore_column['prescale5'     ] = '0x0'
hstore_column['prescale6'     ] = '0x0'
hstore_column['prescale7'     ] = '0x0'
hstore_column['prescale8'     ] = '0x0'
hstore_column['pulse_delay1'  ] = '0x0'
hstore_column['pulse_delay2'  ] = '0x0'
hstore_column['pulse_width1'  ] = '0x0'
hstore_column['pulse_width2'  ] = '0x0'
hstore_column['calib_delay'   ] = '0x0'
hstore_column['deadtime'      ] = '0x0'

writer_api.InsertSubConfigValuesPerCrate(config_name,
                                         config_id,
                                         crate_id,
                                         hstore_column)








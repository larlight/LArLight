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
hstore_column[ 'nchannels'         ] = '2'
hstore_column[ 'top_ch_enable'     ] = '0xffff'
hstore_column[ 'middle_ch_enable'  ] = '0xffff'
hstore_column[ 'bottom_ch_enable'  ] = '0xffff'
hstore_column[ 'cosmic_trig_mult'  ] = '2'
hstore_column[ 'cosmic_trig_thres' ] = '200'
hstore_column[ 'beam_trig_mult'    ] = '2'
hstore_column[ 'beam_trig_thres'   ] = '200'
hstore_column[ 'numi_window_size'  ] = '1500'
hstore_column[ 'numi_window_delay' ] = '20'
hstore_column[ 'numi_gate_size'    ] = '640'
hstore_column[ 'numi_gate_delay'   ] = '640'
hstore_column[ 'bnb_window_size'   ] = '1600'
hstore_column[ 'bnb_window_delay'  ] = '20'
hstore_column[ 'bnb_gate_size'     ] = '103'
hstore_column[ 'bnb_gate_delay'    ] = '256'

for x in xrange(48):
    hstore_column['ch_config_id_%02d' % x] = '0'

# PMT FEM MODULE config ... crate = crate ID
writer_api.InsertSubConfigValues("pmt_fem_mod", # config name
                                 2,             # config ID
                                 0,             # crate   = crate ID  (=0 always for PMT)
                                 1,             # channel = module ID 
                                 hstore_column  # parameter sets
                                 )



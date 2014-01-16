#include "config_reader.h"

struct daq_config_t get_daq_config() {

  struct daq_config_t new_conf;
  new_conf.param[DAQ_CRATE_PMT] = 0;
  new_conf.param[DAQ_CRATE_TPC] = 0;
  new_conf.param[DAQ_READOUT_PMT_NU] = 1;
  new_conf.param[DAQ_READOUT_TPC_NU] = 1;
  new_conf.param[DAQ_READOUT_PMT_SN] = 1;
  new_conf.param[DAQ_READOUT_TPC_SN] = 1;
  new_conf.param[DAQ_READOUT_TRIGGER] = 1;
  new_conf.param[DAQ_READOUT_MONITOR] = 1;
  return new_conf;
};

struct tpc_fem_config_t get_tpc_fem_config(int slot){

  struct tpc_fem_config_t new_conf;
  new_conf.slot=slot;
  new_conf.param[TPC_ADC_READOUT]=1;
  new_conf.param[TPC_DRIFT_SIZE]=20;
  new_conf.param[TPC_ADC_READOUT]=1;
  new_conf.param[TPC_NU_HUFFMAN]=0;
  new_conf.param[TPC_SN_HUFFMAN]=0;
  new_conf.param[TPC_NU_COMP_FACTOR]=0;
  new_conf.param[TPC_SN_COMP_FACTOR]=0;
  new_conf.param[TPC_MODULE_NUM]=2;
  return new_conf;
}

struct xmit_config_t get_xmit_config(int slot) {

  struct xmit_config_t new_conf;
  new_conf.slot = slot;
  new_conf.param[XMIT_NU_BUFFSIZE] = 1000000;
  new_conf.param[XMIT_SN_BUFFSIZE] = 1000000;
  return new_conf;
};

struct trigger_config_t get_trigger_config(int slot) {

  struct trigger_config_t new_conf;
  new_conf.slot = slot;

  new_conf.param[TRIG_MASK0] = 0x0;
  new_conf.param[TRIG_MASK1] = 0x8;
  new_conf.param[TRIG_MASK8] = 0xfd;

  new_conf.param[TRIG_PRESCALE0] = 0;
  new_conf.param[TRIG_PRESCALE1] = 0;
  new_conf.param[TRIG_PRESCALE8] = 0;

  new_conf.param[TRIG_PULSE] = 0;
  new_conf.param[TRIG_PULSE_DELAY1] = 1;
  new_conf.param[TRIG_PULSE_DELAY2] = 200;
  new_conf.param[TRIG_PULSE_WIDTH1] = 1;
  new_conf.param[TRIG_PULSE_WIDTH2] = 100;
  return new_conf;
};

struct pmt_fem_config_t get_pmt_fem_config(int slot)
{
  struct pmt_fem_config_t new_conf;
  new_conf.slot=slot;
  new_conf.param[PMT_ENABLE_UPPER]  = 0xffff;
  new_conf.param[PMT_ENABLE_MIDDLE] = 0xffff;
  new_conf.param[PMT_ENABLE_LOWER]  = 0xffff;

  new_conf.param[PMT_TRIG_WIDTH]        = 5;
  new_conf.param[PMT_TRIG_COSMIC_MULT]  = 1;
  new_conf.param[PMT_TRIG_COSMIC_THRES] = 200;
  new_conf.param[PMT_TRIG_BEAM_MULT]    = 1;
  new_conf.param[PMT_TRIG_BEAM_THRES]   = 1;
  new_conf.param[PMT_BEAM1_NSAMPLE]     = 1500;
  new_conf.param[PMT_BEAM2_NSAMPLE]     = 1500;
  new_conf.param[PMT_BEAM1_WIDTH]       = 102;
  new_conf.param[PMT_BEAM2_WIDTH]       = 102;
  new_conf.param[PMT_BEAM1_DELAY]       = 256;
  new_conf.param[PMT_BEAM2_DELAY]       = 256;

  return new_conf;
};

struct pmt_ch_config_t get_pmt_ch_config(int slot) {
  
  struct pmt_ch_config_t new_conf;
  new_conf.slot=slot;
  int i=0;
  for(i=0; i<48; ++i) {
    new_conf.param[i][PMT_CH_DEADTIME]       = 24;
    new_conf.param[i][PMT_CH_NSAMPLE_COSMIC] = 20;
    new_conf.param[i][PMT_CH_PRECOUNT]       = 3;
    new_conf.param[i][PMT_CH_THRESHOLD0]     = 3;
    new_conf.param[i][PMT_CH_THRESHOLD1]     = 5;
    new_conf.param[i][PMT_CH_THRESHOLD3]     = 10;
  }
  return new_conf;
};



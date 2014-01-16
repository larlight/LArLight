
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include "wdc_defs.h"
#include "wdc_lib.h"
#include "utils.h"
#include "status_strings.h"
#include "samples/shared/diag_lib.h"
#include "samples/shared/wdc_diag_lib.h"
#include "samples/shared/pci_regs.h"
#include "pcie_lib.h"

// Configuration ENUM
enum daq_config_att {

  // RUN Configuration
  DAQ_CRATE_PMT=0,
  DAQ_CRATE_TPC,

  DAQ_READOUT_PMT_NU,
  DAQ_READOUT_TPC_NU,
  DAQ_READOUT_PMT_SN,
  DAQ_READOUT_TPC_SN,

  DAQ_READOUT_TRIGGER,
  DAQ_READOUT_MONITOR,
  DAQ_CONFIG_MAX
};

struct daq_config_t {
  int param[DAQ_CONFIG_MAX];
};

enum tpc_fem_config_att {  

  // TPC Configuration
  TPC_ADC_READOUT=0,
  TPC_DRIFT_SIZE,
  TPC_NU_HUFFMAN,
  TPC_SN_HUFFMAN,
  TPC_NU_COMP_FACTOR,
  TPC_SN_COMP_FACTOR,
  TPC_MODULE_NUM,
  TPC_FEM_CONFIG_MAX
};

struct tpc_fem_config_t {
  int slot;
  int param[TPC_FEM_CONFIG_MAX];
};

// XMIT configuration struct
enum xmit_config_att {

  XMIT_NU_BUFFSIZE,
  XMIT_SN_BUFFSIZE,
  XMIT_CONFIG_MAX
};

struct xmit_config_t {
  
  int slot;
  int param[XMIT_CONFIG_MAX];
};

// Trigger configuration struct
enum trigger_config_att {
  
  TRIG_FRAME_SIZE=0,
  TRIG_MASK0,
  TRIG_MASK1,
  TRIG_MASK8,
  TRIG_PRESCALE0,
  TRIG_PRESCALE1,
  TRIG_PRESCALE8,
  TRIG_PULSE,
  TRIG_PULSE_DELAY1,
  TRIG_PULSE_DELAY2,
  TRIG_PULSE_WIDTH1,
  TRIG_PULSE_WIDTH2,
  TRIGGER_CONFIG_MAX
};

struct trigger_config_t {

  int slot;
  int param[TRIGGER_CONFIG_MAX];
};

enum pmt_fem_config_att {

  PMT_ENABLE_UPPER=0,
  PMT_ENABLE_MIDDLE,
  PMT_ENABLE_LOWER,

  PMT_TRIG_WIDTH,
  PMT_TRIG_COSMIC_MULT,
  PMT_TRIG_COSMIC_THRES,
  PMT_TRIG_BEAM_MULT,
  PMT_TRIG_BEAM_THRES,

  PMT_BEAM1_NSAMPLE,
  PMT_BEAM2_NSAMPLE,
  PMT_BEAM1_WIDTH,
  PMT_BEAM2_WIDTH,
  PMT_BEAM1_DELAY,
  PMT_BEAM2_DELAY,  
  PMT_FEM_CONFIG_MAX
};

struct pmt_fem_config_t {
  int slot;
  int param[PMT_FEM_CONFIG_MAX];
};

enum pmt_ch_config_att {

  PMT_CH_DEADTIME=0,
  PMT_CH_NSAMPLE_COSMIC,
  PMT_CH_PRECOUNT,
  PMT_CH_THRESHOLD0,
  PMT_CH_THRESHOLD1,
  PMT_CH_THRESHOLD3,
  PMT_CH_CONFIG_MAX
};

struct pmt_ch_config_t {
  int slot;
  int param[48][PMT_CH_CONFIG_MAX];
};

struct daq_config_t     get_daq_config();
struct xmit_config_t    get_xmit_config(int slot);
struct trigger_config_t get_trigger_config(int slot);
struct tpc_fem_config_t get_tpc_fem_config(int slot);
struct pmt_fem_config_t get_pmt_fem_config(int slot);
struct pmt_ch_config_t  get_pmt_ch_config(int slot);


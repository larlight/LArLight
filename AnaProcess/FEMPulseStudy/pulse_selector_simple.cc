#ifndef PULSE_SELECTOR_SIMPLE_CC
#define PULSE_SELECTOR_SIMPLE_CC

#include "pulse_selector_simple.hh"

namespace larlight {

  pulse_selector_simple::pulse_selector_simple() : pulse_selector_base() {

    _name="pulse_selector_simple"; 
    _fout=0;
    reset_cuts();
  };

  void pulse_selector_simple::reset_cuts() {

    _cut_tstart        = std::make_pair(-1,2000);
    _cut_tend          = std::make_pair(-1,2000);
    _cut_amp           = std::make_pair(-1,4096);
    _cut_charge        = std::make_pair(-1,4096*2000);
    _cut_pedbase       = std::make_pair(-1,4096);
    _cut_pedrms        = std::make_pair(0,4096);
    _cut_channels      = std::make_pair(0,FEM::INVALID_CH);
    _cut_frame_number  = std::make_pair(0,0xffffffff);
    _cut_sample_number = std::make_pair(0,0xffffffff);
    _cut_npulse        = std::make_pair(0,0xffffffff);
    _cut_sum_charge    = std::make_pair(0,4096*2000*FEM::NUM_PMT_CHANNEL);
    _cut_sum_peak      = std::make_pair(0,4096*FEM::NUM_PMT_CHANNEL);

  }

  bool pulse_selector_simple::select_pulse(const pulse* data){


      UShort_t ch(data->channel_number());

      UInt_t sample   = data->readout_sample_number();
      UInt_t frame    = data->readout_frame_number();
      double t_start  = data->start_time();
      double t_end    = data->end_time();
      double charge   = data->charge();
      double amp      = data->pulse_peak();
      double ped_base = data->ped_mean();
      double ped_rms  = data->ped_rms();

      // Check if this pulse passes the criteria
      if(frame < _cut_frame_number.first || _cut_frame_number.second < frame)
	return false;
      if(sample < _cut_sample_number.first || _cut_sample_number.second < sample)
	return false;
      if(ch < _cut_channels.first || _cut_channels.second < ch)
	return false;
      if(t_start < _cut_tstart.first || _cut_tstart.second < t_start)
	return false;
      if(t_end < _cut_tend.first || _cut_tend.second < t_end)
	return false;
      if(charge < _cut_charge.first || _cut_charge.second < charge)
	return false;
      if(amp < _cut_amp.first || _cut_amp.second < amp)
	return false;
      if(ped_base < _cut_pedbase.first || _cut_pedbase.second < ped_base)
	return false;
      if(ped_rms < _cut_pedrms.first || _cut_pedrms.second < ped_rms)
	return false;    

      return true;
  }

}
#endif

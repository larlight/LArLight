#ifndef PULSE_CC
#define PULSE_CC

#include "pulse.hh"

namespace larlight {

  pulse::pulse(DATA::DATA_TYPE type) : data_base(type) 
  {
    if(_type!=DATA::PMTPulse_FixedWin && 
       _type!=DATA::TPCPulse_FixedWin &&
       _type!=DATA::PMTPulse_ThresWin &&
       _type!=DATA::TPCPulse_ThresWin &&
       _type!=DATA::Pulse ) {
      
      Message::send(MSG::ERROR,__FUNCTION__,Form("Invalid data type specification (=%d)!",_type));

      Message::send(MSG::ERROR,__FUNCTION__,"Assuming Generic EventPulse...");

      _type=DATA::Pulse;

    }
    clear_data();
  }

  void pulse::clear_data() {

    data_base::clear_data();
    _disc_id=FEM::DISC_MAX;
    _readout_frame_number=FEM::INVALID_WORD;
    _readout_sample_number=FEM::INVALID_WORD;
    _channel_number=FEM::INVALID_CH;
    _ped_mean=-1;
    _ped_rms=-1;
    _charge=-1;
    _start_time=-1;
    _end_time=-1;
    _max_time=-1;

  }

  event_pulse::event_pulse(DATA::DATA_TYPE type) : std::vector<larlight::pulse>(),
						   data_base(type) 
  {
    if(_type!=DATA::PMTPulse_FixedWin && 
       _type!=DATA::TPCPulse_FixedWin &&
       _type!=DATA::PMTPulse_ThresWin &&
       _type!=DATA::TPCPulse_ThresWin &&
       _type!=DATA::Pulse ) {
      
      Message::send(MSG::ERROR,__FUNCTION__,Form("Invalid data type specification (=%d)!",_type));

      Message::send(MSG::ERROR,__FUNCTION__,"Assuming Generic EventPulse...");

      _type=DATA::Pulse;

    }
    clear_data();
  };			   

  void event_pulse::clear_data(){

    data_base::clear_data();
    clear();
    _sum_charge=-1;
    _sum_peak=-1;
    _npulse=0;

  }

  void event_pulse::calculate_sum(){

    std::vector<pulse>::const_iterator iter(this->begin());
    _sum_charge=0;
    _sum_peak=0;
    _npulse=0;
    while(iter!=this->end()){

      _sum_charge+=(*iter).charge();
      _sum_peak+=(*iter).pulse_peak();
      _npulse++;
    }

  }
}
#endif 

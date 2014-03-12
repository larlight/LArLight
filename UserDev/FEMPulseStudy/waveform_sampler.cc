#ifndef WAVEFORM_SAMPLER_CC
#define WAVEFORM_SAMPLER_CC

#include "waveform_sampler.hh"

namespace larlight {

  waveform_sampler::waveform_sampler() : _sel_info(), _hWF_map()
  { 
    _name="waveform_sampler"; 
    _fout=0;
    _selector=0;
    _wf_length = 0;
  };

  bool waveform_sampler::initialize() {

    if(!(_selector)) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    "pulse selection algorithm has not been set! Aborting...");

      return false;
    }

    if(_wf_length < 1) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    "sample waveform length is set to < 1! Aborting...");

      return false;
    }

    _hWF_map.clear();

    return true;
  }

  void waveform_sampler::clear_event(){

    _sel_info.clear();

  }

  bool waveform_sampler::analyze(storage_manager* storage) {

    event_pulse *pulses = (event_pulse*)(storage->get_data(_selector->pulse_type()));

    event_fifo  *wfs    = (event_fifo*)(storage->get_data(DATA::PMTFIFO));

    if(!pulses){

      Message::send(MSG::ERROR,__FUNCTION__,
		    "Necessary input pulses not found!");

      return false;

    }

    if(!wfs){

      Message::send(MSG::ERROR,__FUNCTION__,
		    "Necessary input raw waveform not found!");

      return false;

    }

    clear_event();

    if(!(_selector->analyze(storage))) return false;

    //
    // Log ch number, frame id, sample number of selected pulse
    //
    for(auto pulse : *pulses){

      if(!(_selector->select_pulse(&pulse))) continue;

      UShort_t ch     = pulse.channel_number();
      UInt_t   frame  = pulse.readout_frame_number();
      UInt_t   sample = pulse.readout_sample_number();

      if(_sel_info.find(ch)==_sel_info.end()) 

	_sel_info[ch] = std::map<UInt_t,std::set<UInt_t> >();

      if(_sel_info[ch].find(frame) == _sel_info[ch].end() )

	_sel_info[ch][frame] = std::set<UInt_t>(); 

      if(_sel_info[ch][frame].find(sample) == _sel_info[ch][frame].end())

	_sel_info[ch][frame].insert(sample);

    }

    //
    // Loop over waveforms that matches with the selected IDs
    //
    for(auto wf : *wfs){

      UShort_t ch     = wf.channel_number();
      UInt_t   frame  = wf.readout_frame_number();
      UInt_t   sample = wf.readout_sample_number_RAW();

      if(_sel_info.find(ch) == _sel_info.end()) continue;

      if(_sel_info[ch].find(frame) == _sel_info[ch].end()) continue;

      if(_sel_info[ch][frame].find(sample) == _sel_info[ch][frame].end()) continue;

      sample_waveform(&wf);

    }  

    /*
    for(pmt_wf_collection::iterator iter(wfs->begin());
	iter != wfs->end();
	++iter){

      UShort_t ch     = (*iter).channel_number();
      UInt_t      frame  = (*iter).readout_frame_number();
      UInt_t      sample = (*iter).readout_sample_number_RAW();

      if(_sel_info.find(ch) == _sel_info.end()) continue;

      if(_sel_info[ch].find(frame) == _sel_info[ch].end()) continue;

      if(_sel_info[ch][frame].find(sample) == _sel_info[ch][frame].end()) continue;

      sample_waveform(&(*iter));

    }  
    */


    return true;
  }

  void waveform_sampler::sample_waveform(const fifo *wf){

    UShort_t ch(wf->channel_number());

    int nbins = (_wf_length < (int)(wf->size())) ? _wf_length : (int)(wf->size());

    _fout->cd();
    if(_hWF_map.find(ch)==_hWF_map.end()) {

      TString pulse_name(Form("%s",DATA::DATA_TREE_NAME[_selector->pulse_type()].c_str()));
      pulse_name.ReplaceAll("pulse_","");
      pulse_name.ReplaceAll("_window","");    

      _hWF_map[ch] = new TH2D(Form("hWF_%s_Ch%02d",pulse_name.Data(),ch),
			      Form("Raw Waveform Sample for Ch %d; Timeslice; ADC",ch),
			      nbins, -0.5, ((double)nbins)-0.5, 4096, -0.5,4095.5);

    }

    for(int i=0; i<nbins; i++)

      _hWF_map[ch]->Fill((double)i, (double)(wf->at(i)));

  }

  bool waveform_sampler::finalize() {

    if(_fout){

      _fout->cd();

      for(std::map<UShort_t,TH2D*>::const_iterator iter(_hWF_map.begin());
	  iter!=_hWF_map.end();
	  ++iter){

	(*iter).second->Write();

      }

    }

    return true;
  }
}

#endif

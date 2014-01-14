#ifndef PULSE_ANALYZER_CC
#define PULSE_ANALYZER_CC

#include "pulse_analyzer.hh"

namespace larlight {

  pulse_analyzer::pulse_analyzer() { 
    _name="pulse_analyzer"; 
    _pulse_type=DATA::Pulse;
  }

  bool pulse_analyzer::initialize() {

    TString tree_name(Form("%s_tree",DATA::DATA_TREE_NAME[_pulse_type].c_str()));
    tree_name.ReplaceAll("pulse_","");
    tree_name.ReplaceAll("_window","");

    _fout->cd();

    _ch_tree = new TTree(tree_name.Data(),"");
    _ch_tree->Branch("event_id",&_event_id,"event_id/I");
    _ch_tree->Branch("frame_id",&_ch_frame_id,"frame_id/I");
    _ch_tree->Branch("sample_id",&_ch_sample_id,"sample_id/I");
    _ch_tree->Branch("disc_id",&_disc_id,"disc_id/I");
    _ch_tree->Branch("ch",&_ch,"ch/I");
    _ch_tree->Branch("charge",&_wf_charge,"charge/D");
    _ch_tree->Branch("peak",&_wf_peak,"peak/D");
    _ch_tree->Branch("t_start",&_wf_ts,"t_start/D");
    _ch_tree->Branch("t_max",&_wf_tm,"t_max/D");
    _ch_tree->Branch("t_end",&_wf_te,"t_end/D");
    _ch_tree->Branch("ped_mean",&_ped_mean,"ped_mean/D");
    _ch_tree->Branch("ped_rms",&_ped_rms,"ped_rms/D");
    _ch_tree->Branch("nsample",&_nsample,"nsample/I");

    return true;
  }

  bool pulse_analyzer::analyze(storage_manager* storage) {

    event_pulse *pulses = (event_pulse*)(storage->get_data(_pulse_type));
    event_fifo   *wfs    = (event_fifo*)(storage->get_data(DATA::PMTFIFO));

    if(pulses->size()==0) {
      Message::send(MSG::ERROR,__FUNCTION__,"No Pulse Found!");
      return false;
    }
    if(wfs->size()==0) {
      Message::send(MSG::ERROR,__FUNCTION__,"No Waveforms Found!");
      return false;
    }

    // Fill map for this event
    for(event_pulse::const_iterator iter(pulses->begin());
	iter!=pulses->end();
	++iter){
      _ch           = (*iter).channel_number();
      _ch_frame_id  = (*iter).frame_id();
      _ch_sample_id = (*iter).timeslice();
      _disc_id      = (int)((*iter).disc_id());
      _wf_charge    = (*iter).charge();
      _wf_peak      = (*iter).pulse_peak();
      _wf_ts        = (*iter).start_time();
      _wf_tm        = (*iter).max_time();
      _wf_te        = (*iter).end_time();
      _ped_mean     = (*iter).ped_mean();
      _ped_rms      = (*iter).ped_rms();
      _ch_tree->Fill();

    }

    return true;
  }

  bool pulse_analyzer::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //amplitude->Write();
    _fout->cd();
    _ch_tree->Write();
    return true;
  }

}
#endif

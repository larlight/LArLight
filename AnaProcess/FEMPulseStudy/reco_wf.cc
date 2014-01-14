#ifndef RECO_WF_CC
#define RECO_WF_CC

#include "reco_wf.hh"
namespace larlight {

  reco_wf::reco_wf() : _wf_histo(0) {
    _frame_width=4096;
    _name="reco_wf";
    reset();

  }

  void reco_wf::reset(){

    clear_event();
  }

  void reco_wf::clear_event(){

    if(_wf_histo)
      delete _wf_histo;
    _wf_histo=0;
    _channels.clear();
    _wf_map.clear();
    _ref_frame=0;
    _ref_slice=0;
    _event_id=0;
  }

  bool reco_wf::initialize() {

    if(!_frame_width){
      Message::send(MSG::ERROR,__FUNCTION__,"Frame width unspecified! ");
      return false;
    }

    return true;

  };

  bool reco_wf::analyze(storage_manager* storage){

    event_fifo *data = (event_fifo*)(storage->get_data(DATA::PMTFIFO));
    clear_event();

    if(!(data->size()))
      return true;

    _event_id=data->event_id();

    UInt_t rel_frame=0;
    UInt_t rel_slice=0;
    UInt_t wf_length=0;

    // 
    // Loop over event (collection of channel waveforms) to first
    // identify the maximum length of waveform vector to be stored.
    //
    std::set<size_t> skip_index; // a set of index in pmt_wf_collection array to be skipped
    for(size_t i=0; i<data->size(); ++i){

      const fifo* pmt_data = &(data->at(i));

      if(pmt_data->size()<1) {
	skip_index.insert(i);
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Detected 0-length waveform (Event=%d ... Ch.=%d)! Skipping.",data->event_id(),pmt_data->channel_number()));
	continue;
      }

      if(!(_ref_frame) || _ref_frame>pmt_data->channel_frame_id())
	_ref_frame = pmt_data->channel_frame_id();

      rel_frame = pmt_data->channel_frame_id() - _ref_frame;
      rel_slice = rel_frame * _frame_width + pmt_data->timeslice();

      if(!(_ref_slice) || _ref_slice>rel_slice)
	_ref_slice=rel_slice;

      if(!(wf_length) || wf_length<(rel_slice+pmt_data->size()))
	wf_length=rel_slice+pmt_data->size();

      if(_channels.find(pmt_data->channel_number())==_channels.end())
	_channels.insert(pmt_data->channel_number());

    }

    //
    // Loop over again. This time create a channel waveform container
    // and store waveform data inside. By default set all waveform
    // vector contents to be 0. We write out all elements, whether data
    // exists or not, for the time span of the first and last received
    // adc count among all channels.
    //
    for(size_t i=0; i<data->size(); ++i){

      if(skip_index.find(i)!=skip_index.end()) continue;

      const fifo* pmt_data = &(data->at(i));
      UInt_t ch=pmt_data->channel_number();    

      if(_wf_map.find(ch)==_wf_map.end())
	_wf_map[ch]=std::vector<UShort_t>(wf_length-_ref_slice+1,0);

      for(size_t j=0; j<pmt_data->size(); ++j){

	rel_frame = pmt_data->channel_frame_id() - _ref_frame;
	rel_slice = rel_frame * _frame_width + pmt_data->timeslice() + j;

	_wf_map[ch][rel_slice - _ref_slice] = pmt_data->at(j);
      }
    }

    // 
    // Summary report if verbosity is set
    //
    if(_verbosity[MSG::INFO]){
      sprintf(_buf,"Event = %-4d ... %-2zd channels read out",data->event_id(),_channels.size());
      Message::send(MSG::INFO,__FUNCTION__,_buf);
    }

    return true;
  }

  const std::vector<UShort_t>* reco_wf::get_ch_waveform(UInt_t ch) {

    if(_wf_map.find(ch)==_wf_map.end())
      return 0;
    else
      return &(_wf_map[ch]);
  }


  bool reco_wf::finalize(){

    return true;

  }

  TH2D* reco_wf::get_histo(){

    if(_wf_histo)
      return _wf_histo;

    if(_wf_map.size()==0) return 0;

    // Create a histogram
    int ctr=0;
    for(std::set<UInt_t>::const_iterator iter(_channels.begin());
	iter!=_channels.end();
	++iter){

      std::vector<UShort_t>* wf=&(_wf_map[(*iter)]);

      if(!(_wf_histo)){
	// If histogram does not yet exist, create one.
	// This hsould be called in the 1st loop.

	_wf_histo=new TH2D(Form("hWF_Event%05d", _event_id),
			   Form("All waveform combined for event %-5d; ; Time-slice; ADC",_event_id),
			   _channels.size(), 0, _channels.size(),                  // X-axis = channels
			   wf->size(), _ref_slice, (_ref_slice+wf->size()) // Y-axis = time slice
			   );
      }

      ctr++;

      for(unsigned int index=0; index<wf->size(); index++)

	_wf_histo->SetBinContent(ctr,index+1,(double)(wf->at(index)));

      _wf_histo->GetXaxis()->SetBinLabel(ctr,Form("Ch. %d",(*iter)));
    }

    if(_wf_histo){
      _wf_histo->GetXaxis()->SetTicks("-");
      _wf_histo->GetXaxis()->LabelsOption("h");
    }

    return _wf_histo;

  }
}
#endif

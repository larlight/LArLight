#ifndef GEORGIA_TIMING1_CC
#define GEORGIA_TIMING1_CC

#include "georgia_timing1.hh"

namespace larlight {

  bool georgia_timing1::initialize() {
    
    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you need, you have an output root file pointer "_fout".
    
    pmt_nu_evno = new TH1D("evno","Event number",10000,0,10000);
    pmt_nu_evfrm = new TH1D("evfrm","Event frame",100000,0,100000);
    pmt_nu_add = new TH1D("add","Module address",22,0,22);
    pmt_nu_mod = new TH1D("mod","Module ID",22,0,22);
    pmt_nu_trfrm = new TH1D("trfrm_nu","Trigger frame",100000,0,100000);
    pmt_nu_trslc = new TH1D("trslc_nu","Trigger timeslice",10000,0,10000);
    pmt_nu_ch = new TH1D("ch","Channel numbers read over all events",64,0,64);
    pmt_nu_nch = new TH1D("nch","Number of channels read per event",64,0,64);
    pmt_nu_nrd = new TH1D("nrd","Number of reads per channel number",64,0,64);
    pmt_nu_rdtype = new TH2D("rdtype","Number of reads per channel number per type",64,0,64,5,0,5);
    
    trig_trfrm = new TH1D("trfrm_tr","Trigger frame",100000,0,100000);
    trig_trslc = new TH1D("trslc_tr","Trigger timeslice",10000,0,10000);
    trig_trid = new TH1D("trid","Trigger Type",10,0,10);
    trig_pmid = new TH1D("pmid","PMT Trigger Type",10,0,10);
    return true;
    
  }
  
  bool georgia_timing1::analyze(storage_manager* storage) {
    
    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "data" pointer which contains 
    // event-wise data. For a reference of pmt_wf_collection class instance, 
    // see the class index in the documentation.
    // 
    // Example to print out event id on screen...
    //
    
    const event_fifo *event_wf = (event_fifo*)(storage->get_data(DATA::PMTFIFO));
    std::cout << Form("Event ID: %d",event_wf->event_id()) << std::endl;
    //
    
    _nch=0;
    
    //loop over all channels
    for (size_t i=0; i<event_wf->size(); ++i){
      
      const fifo* pmt_data = (&(event_wf->at(i)));
      if(pmt_data->size()<1){
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Found 0-length waveform: Event %d ... Ch. %d",event_wf->event_id(),pmt_data->channel_number()));
	continue;
      }
      _nch++;
      //ch_histo->Fill( pmt_data->channel_number() );
      
    }
    //nch_histo->Fill(_nch);
    
    const trigger* trig_data = (trigger*)(storage->get_data(DATA::Trigger));
    //  std::cout << trig_data->trig_id()<< std::endl;
    std::cout << Form("Event ID: %d",trig_data->trig_id()) << std::endl;
    
    return true;
    
  }
  
  bool georgia_timing1::finalize() {
    
    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    
    if(_fout) {
      _fout->cd();
      
      //ch_histo->Write();
      //nch_histo->Write();
    }
    return true;
  }
}
#endif

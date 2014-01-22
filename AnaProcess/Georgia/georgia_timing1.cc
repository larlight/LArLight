#ifndef GEORGIA_TIMING1_CC
#define GEORGIA_TIMING1_CC

#include "georgia_timing1.hh"

namespace larlight {

  bool georgia_timing1::initialize() {
    
    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you need, you have an output root file pointer "_fout".
    
    pmt_nu_evno = new TH1D("pmt_nu_evno","Event number",1000,0,1000);
    pmt_nu_evfrm = new TH1D("pmt_nu_evfrm","Event frame",10000,0,10000);
    pmt_nu_add = new TH1D("pmt_nu_add","Module address",22,0,22);
    pmt_nu_mod = new TH1D("pmt_nu_mod","Module ID",22,0,22);
    pmt_nu_trfrm = new TH1D("pmt_nu_trfrm","Trigger frame",10000,0,10000);
    pmt_nu_trslc = new TH1D("pmt_nu_trslc","Trigger timeslice",4*102400,-102400,3*102400);
    pmt_nu_ch = new TH1D("pmt_nu_ch","Channel numbers read over all events",64,0,64);
    pmt_nu_nch = new TH1D("pmt_nu_nch","Number of channels read per event",64,0,64);
    pmt_nu_nrd = new TH1D("pmt_nu_nrd","Number of reads per channel number",64,0,64);
    pmt_nu_rdtype = new TH2D("pmt_nu_rdtype","Number of reads per channel number per type",64,0,64,5,0,5);

    pmt_nu_frdiff = new TGraph();
    pmt_nu_frdiff->SetName("pmt_nu_frdiff");
    pmt_nu_frdiff->SetTitle("Event Fr (FEM) - Trigger Fr (FEM) vs Event No (FEM)");

    pmt_nu_slcdiff = new TGraph();
    pmt_nu_slcdiff->SetName("pmt_nu_slcdiff");
    pmt_nu_slcdiff->SetTitle("Event Time (FEM) - Trigger Time (FEM) vs Event No (FEM)");
    
    trig_trfrm = new TH1D("trig_trfrm","Trigger frame",10000,0,10000);
    trig_trslc = new TH1D("trig_trslc","Trigger timeslice",4*102400,-102400,3*102400);
    trig_trid = new TH1D("trig_trid","Trigger Event Number",1000,0,1000);
    trig_pmid = new TH1D("trig_pmid","PMT Trigger Type",10,0,10);
    trig_trpc = new TH1D("trig_trpc","PC Trigger",10,0,10);
    trig_extrn = new TH1D("trig_extrn","EXT Trigger",10,0,10);
    trig_actv = new TH1D("trig_actv","Trigger Active",10,0,10);
    trig_gate1 = new TH1D("trig_gate1","Gate 1 Trigger",10,0,10);
    trig_gate2 = new TH1D("trig_gate2","Gate 2 Trigger",10,0,10);
    trig_veto = new TH1D("trig_veto","VETO",10,0,10);
    trig_calib = new TH1D("trig_calib","Calibration",10,0,10);
    trig_rem64 = new TH1D("trig_rem64","64MHz remainder",4,0,4);
    trig_rem16 = new TH1D("trig_rem16","16MHz remainder",8,0,8);

    trig_frdiff = new TGraph();
    trig_frdiff->SetName("trig_frdiff");
    trig_frdiff->SetTitle("Trigger Fr (TRG) - Trigger Fr (FEM) vs Event No (FEM)");

    trig_slcdiff = new TGraph();
    trig_slcdiff->SetName("trig_slcdiff");
    trig_slcdiff->SetTitle("Trigger Time (TRG) - Trigger Time (FEM) vs Event No (FEM)");

    k = 0;

    return true;
    
  }
  
  bool georgia_timing1::analyze(storage_manager* storage) {

    //
    // First check if data exists
    //
    // PMT data
    const event_fifo *event_wf = (event_fifo*)(storage->get_data(DATA::PMTFIFO));    
    const trigger* trig_data = (trigger*)(storage->get_data(DATA::Trigger));
    if(!trig_data) {

      print(MSG::ERROR,__FUNCTION__,"Trigger data not found!");

      return false;

    }else if(!event_wf) {

      print(MSG::ERROR,__FUNCTION__,"PMT data not found!");

      return false;

    }
    
    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "data" pointer which contains 
    // event-wise data. For a reference of pmt_wf_collection class instance, 
    // see the class index in the documentation.
    // 
    // Example to print out event id on screen...
    //
    

    //    std::cout << Form("Event ID: %d",event_wf->event_number()) << std::endl;
    pmt_nu_evtno = event_wf->event_number();

    pmt_nu_evno->Fill(event_wf->event_number());
    pmt_nu_evfrm->Fill(event_wf->event_frame_number());
    pmt_nu_add->Fill(event_wf->module_address());
    pmt_nu_mod->Fill(event_wf->module_id());
    pmt_nu_trfrm->Fill(event_wf->fem_trig_frame_number());
    pmt_nu_trslc->Fill(event_wf->fem_trig_sample_number_64MHz());

    pmt_nu_frdiff->SetPoint(k,event_wf->event_number(),double(event_wf->event_frame_number())-double(event_wf->fem_trig_frame_number()));

    pmt_nu_slcdiff->SetPoint( k,
			      event_wf->event_number(),
			      double(event_wf->event_frame_number())*102400 - 
			      (double(event_wf->fem_trig_frame_number())*102400+double(event_wf->fem_trig_sample_number_64MHz())) );

    _nch=0;
    _nrd=0;

    //loop over all channels
    for (size_t i=0; i<event_wf->size(); ++i){
      
      const fifo* pmt_data = (&(event_wf->at(i)));
      if(pmt_data->size()<1){
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Found 0-length waveform: Event %d ... Ch. %d",event_wf->event_number(),pmt_data->channel_number()));
	continue;
      }
      _nch++;
      _nrd++;
      pmt_nu_ch->Fill( pmt_data->channel_number() );
      pmt_nu_rdtype->Fill( pmt_data->channel_number(),pmt_data->disc_id() );
    }
    pmt_nu_nch->Fill(_nch);
    pmt_nu_nrd->Fill(_nrd);


    //    std::cout << Form("Event ID: %d",trig_data->trig_number()) << std::endl;
    trig_evtno = trig_data->trig_number();
    
    trig_trfrm->Fill(trig_data->trig_frame_number());
    trig_trslc->Fill(trig_data->trig_sample_number_2MHz());
    trig_trid->Fill(trig_data->trig_number());
    trig_pmid->Fill(trig_data->pmt_data());
    trig_trpc->Fill(trig_data->trig_pc());
    trig_extrn->Fill(trig_data->trig_ext());
    trig_actv->Fill(trig_data->active());
    trig_gate1->Fill(trig_data->gate1_in());
    trig_gate2->Fill(trig_data->gate2_in());
    trig_veto->Fill(trig_data->veto_in());
    trig_calib->Fill(trig_data->calib());
    trig_rem64->Fill(trig_data->remainder_64MHz());
    trig_rem16->Fill(trig_data->remainder_16MHz());
    
    if (trig_evtno==pmt_nu_evtno){
      
      //compare trigger frame numbers
      trig_frdiff->SetPoint(k,
			    event_wf->event_number(),
			    double(trig_data->trig_frame_number())-double(event_wf->fem_trig_frame_number()));
      
      trig_slcdiff->SetPoint( k,
			      event_wf->event_number(),
			      (double(trig_data->trig_frame_number())*102400+double(trig_data->trig_sample_number_64MHz())) - 
			      (double(event_wf->fem_trig_frame_number())*102400+double(event_wf->fem_trig_sample_number_64MHz())) );
      
    }
    else {
      std::cout << "WARNING! event numbers from PMT and Trigger streams don't match!" << std::endl;
      std::cout << "  PMT Event ID: " << event_wf->event_number() << std::endl;
      std::cout << "  TRG Event ID: " << trig_data->trig_number() << std::endl;
    }

    k++;

    return true;
    
  }
  
  bool georgia_timing1::finalize() {
    
    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    
    if(_fout) {
      _fout->cd();
      
      //output histograms

      pmt_nu_evno->Write();
      pmt_nu_evfrm->Write();
      pmt_nu_add->Write();
      pmt_nu_mod->Write();
      pmt_nu_trfrm->Write();
      pmt_nu_trslc->Write();
      pmt_nu_ch->Write();
      pmt_nu_nch->Write();
      pmt_nu_nrd->Write();
      pmt_nu_rdtype->Write("KEY_discID_vs_ch");
    
      pmt_nu_frdiff->Write("KEY_FEM_evtfrm_vs_trfrm");
      pmt_nu_slcdiff->Write("KEY_FEM_evtslc_vs_trslc");

      trig_trfrm->Write();
      trig_trslc->Write();
      trig_trid->Write();
      trig_pmid->Write();
      trig_trpc->Write();
      trig_extrn->Write();
      trig_actv->Write();
      trig_gate1->Write();
      trig_gate2->Write();
      trig_veto->Write();
      trig_calib->Write();
      trig_rem64->Write();
      trig_rem16->Write();
      
      trig_frdiff->Write("KEY_TRG_trgfrm_vs_trfrm");
      trig_slcdiff->Write("KEY_TRG_trgslc_vs_trslc");

    }
    return true;
  }
}
#endif

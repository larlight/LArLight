#ifndef GEORGIALARTF_CC
#define GEORGIALARTF_CC

#include "GeorgiaLArTF.hh"

namespace larlight {

  bool GeorgiaLArTF::initialize() {

    //FEM: Histograms of variables available through event headers, over full run (unique in each event)
    pmt_nu_evno = new TH1D("pmt_nu_evno","FEM Event number",1000,0,1000);
    pmt_nu_evfrm = new TH1D("pmt_nu_evfrm","FEM Event frame",10000,0,10000);
    pmt_nu_add = new TH1D("pmt_nu_add","Module address",22,0,22);
    pmt_nu_mod = new TH1D("pmt_nu_mod","Module ID",22,0,22);
    pmt_nu_trfrm = new TH1D("pmt_nu_trfrm","FEM Trigger frame",10000,0,10000);
    pmt_nu_trslc = new TH1D("pmt_nu_trslc","FEM Trigger timeslice",4*samples_per_frame,-samples_per_frame,3*samples_per_frame);

    //FEM: Histograms of channel numbers read, number of reads, and type of reads (can be multiple per event)
    pmt_nu_ch = new TH1D("pmt_nu_ch","Channel numbers read",64,0,64);
    pmt_nu_nrd = new TH1D("pmt_nu_nrd","Number of reads per event",64,0,64);
    pmt_nu_rdtype = new TH2D("pmt_nu_rdtype","Number of reads per channel number per type",64,0,64,5,0,5);

    //FEM: Relative frame differences (unique in each event)
    pmt_nu_frdiff = new TGraph();
    pmt_nu_frdiff->SetTitle("Event Fr (FEM) - Trigger Fr (FEM) vs Event No (FEM);;");

    //FEM: Relative timeslice differences (unique in each event)
    pmt_nu_slcdiff = new TGraph();
    pmt_nu_slcdiff->SetTitle("Event Time (FEM) - Trigger Time (FEM) vs Event No (FEM);;");

    //TRIG: Variables available through trigger stream (unique in each event)
    trig_trfrm = new TH1D("trig_trfrm","Trigger frame",10000,0,10000);
    trig_trslc = new TH1D("trig_trslc","Trigger timeslice",4*samples_per_frame,-samples_per_frame,3*samples_per_frame);
    trig_trid = new TH1D("trig_trid","Trigger Event Number",1000,0,1000);
    trig_pmid = new TH1D("trig_pmid","PMT Trigger Type",10,0,10);
    trig_trpc = new TH1D("trig_trpc","PC Trigger",10,0,10);
    trig_extrn = new TH1D("trig_extrn","EXT Trigger",10,0,10);
    trig_actv = new TH1D("trig_actv","ACTIVE",10,0,10);
    trig_gate1 = new TH1D("trig_gate1","Gate 1",10,0,10);
    trig_gate2 = new TH1D("trig_gate2","Gate 2",10,0,10);
    trig_veto = new TH1D("trig_veto","VETO",10,0,10);
    trig_calib = new TH1D("trig_calib","Calibration",10,0,10);
    trig_rem64 = new TH1D("trig_rem64","Trigger 64MHz remainder",10,0,10);
    trig_rem16 = new TH1D("trig_rem16","Trigger 16MHz remainder",10,0,10);


    //key plots
    //fill only if event numbers agree
    key_trig_type_vs_evtno = new TGraph();
    key_trig_type_vs_trfrm = new TGraph();
    key_ntrigs_vs_evtno = new TGraph();
    key_fem_trig_trfrm_vs_trfrm = new TGraph();
    key_fem_trig_evfrm_vs_trfrm = new TGraph();
    key_fem_trig_trfrmdiff_vs_evtno = new TGraph();
    key_fem_trig_trfrmdiff_vs_trfrm = new TGraph();
    key_fem_evfrm_vs_trfrm = new TGraph();

    return true;

  }
  
  bool GeorgiaLArTF::analyze(storage_manager* storage) {

    //access trigger board data and FEM (pmt neutrino stream) data
    auto event_wf = (event_pmtfifo*)(storage->get_data(DATA::PMTFIFO));    
    auto trig_data = (trigger*)(storage->get_data(DATA::Trigger));
    if(!trig_data) {
      print(MSG::ERROR,__FUNCTION__,"Trigger data not found!");
      return false;

    } else if(!event_wf) {
      print(MSG::ERROR,__FUNCTION__,"PMT data not found!");
      return false;
    }

    trig_type.clear();

    //FEM: fill histograms of event header variables
    pmt_nu_evno->Fill(event_wf->event_number());
    pmt_nu_evfrm->Fill(event_wf->event_frame_number());
    pmt_nu_add->Fill(event_wf->module_address());
    pmt_nu_mod->Fill(event_wf->module_id());
    pmt_nu_trfrm->Fill(event_wf->fem_trig_frame_number());
    pmt_nu_trslc->Fill(event_wf->fem_trig_sample_number_64MHz());

    pmt_nu_frdiff->SetPoint(pmt_nu_frdiff->GetN(),
			    event_wf->event_number(),
			    double(event_wf->event_frame_number())-double(event_wf->fem_trig_frame_number()));
    
    pmt_nu_slcdiff->SetPoint(pmt_nu_slcdiff->GetN(),
			     event_wf->event_number(),
			     double(event_wf->event_frame_number())*samples_per_frame - 
			     (double(event_wf->fem_trig_frame_number())*samples_per_frame+double(event_wf->fem_trig_sample_number_64MHz())) );

    //FEM: fill histograms of readout header variables
    //loop over all readouts in this event
    int nrd=0;
    for (size_t i=0; i<event_wf->size(); ++i){      
      const pmtfifo* pmt_data = (&(event_wf->at(i)));
      if(pmt_data->size()<1){
	Message::send(MSG::ERROR,__FUNCTION__,
                      Form("Found 0-length waveform: Event %d ... Ch. %d",event_wf->event_number(),pmt_data->channel_number()));
        continue;
      }
      nrd++;
      pmt_nu_ch->Fill( pmt_data->channel_number() );//channel numbers read
      pmt_nu_rdtype->Fill( pmt_data->channel_number(),pmt_data->disc_id() );
    }
    //    printf("Found %d reads in this event\n",nrd);
    //    printf("pmt evno\ttrig evno\tfem trigfr\ttrig trigfr\tfem evfr\n");
    printf("%d\t%d\t%d\t%d\t%d\t%d\n",int(event_wf->module_id()),event_wf->event_number(),trig_data->trig_number(),event_wf->fem_trig_frame_number(),trig_data->trig_frame_number(),event_wf->event_frame_number());

    pmt_nu_nrd->Fill(nrd);//number of reads per event


    //TRIG: fill histograms of event variables
    trig_trfrm->Fill(trig_data->trig_frame_number());
    trig_trslc->Fill(trig_data->trig_sample_number_2MHz()*32+trig_data->remainder_16MHz()*4+trig_data->remainder_64MHz());
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

    //get trigger type:
    if (trig_data->gate1_in()==1)trig_type.push_back(1);//gate1
    if (trig_data->gate2_in()==1)trig_type.push_back(2);//gate2
    if (trig_data->pmt_data()!=0) trig_type.push_back(3);//pmt (any)
    if (trig_data->trig_pc()==1)trig_type.push_back(4);//internal  
    if (trig_data->calib()==1)trig_type.push_back(5);//calib
    if (trig_data->trig_ext()==1)trig_type.push_back(6);//ext

    
    if ((event_wf->event_number() == trig_data->trig_number()) && event_wf->event_number()!=0){

      std::cout << "new set\n";
      //      printf("pmt evno\ttrig evno\tfem trigfr\ttrig trigfr\tfem evfr\n");
      printf("%d\t%d\t%d\t%d\t%d\n",event_wf->event_number(),trig_data->trig_number(),event_wf->fem_trig_frame_number()-trig_data->trig_frame_number(),trig_data->trig_frame_number()-trig_data->trig_frame_number(),event_wf->event_frame_number()-trig_data->trig_frame_number());
      for (int i=0; i<int(trig_type.size()); i++){
	key_trig_type_vs_evtno->SetPoint(key_trig_type_vs_evtno->GetN(),
					 double(event_wf->event_number()),
					 double(trig_type[i])); 
	key_trig_type_vs_trfrm->SetPoint(key_trig_type_vs_trfrm->GetN(),
					 double(trig_data->trig_frame_number()),
					 double(trig_type[i]));
      }

      key_ntrigs_vs_evtno->SetPoint(key_ntrigs_vs_evtno->GetN(),
				    double(event_wf->event_number()),
				    double(trig_type.size()));

      key_fem_trig_trfrm_vs_trfrm->SetPoint(key_fem_trig_trfrm_vs_trfrm->GetN(),
					    double(trig_data->trig_frame_number()),
					    double(event_wf->fem_trig_frame_number()));
      key_fem_trig_evfrm_vs_trfrm->SetPoint(key_fem_trig_evfrm_vs_trfrm->GetN(),
					    double(trig_data->trig_frame_number()),
					    double(event_wf->event_frame_number()));

      key_fem_trig_trfrmdiff_vs_evtno->SetPoint(key_fem_trig_trfrmdiff_vs_evtno->GetN(),
						double(event_wf->event_number()),
						double(event_wf->fem_trig_frame_number())-double(trig_data->trig_frame_number()));
      key_fem_trig_trfrmdiff_vs_trfrm->SetPoint(key_fem_trig_trfrmdiff_vs_trfrm->GetN(),
						double(trig_data->trig_frame_number()),
						double(event_wf->fem_trig_frame_number())-double(trig_data->trig_frame_number()));

      key_fem_evfrm_vs_trfrm->SetPoint(key_fem_evfrm_vs_trfrm->GetN(),
				       double(event_wf->fem_trig_frame_number()),
				       double(event_wf->event_frame_number()));
    }

    return true;
  }

  bool GeorgiaLArTF::finalize() {

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
      
      //key plots
      key_trig_type_vs_evtno->Write("key_trig_type_vs_evtno");
      key_trig_type_vs_trfrm->Write("key_trig_type_vs_trfrm");
      key_ntrigs_vs_evtno->Write("key_ntrigs_vs_evtno");
      key_fem_trig_trfrm_vs_trfrm->Write("key_fem_trig_trfrm_vs_trfrm");
      key_fem_trig_evfrm_vs_trfrm->Write("key_fem_trig_evfrm_vs_trfrm");
      key_fem_trig_trfrmdiff_vs_evtno->Write("key_fem_trig_trfrmdiff_vs_evtno");
      key_fem_trig_trfrmdiff_vs_trfrm->Write("key_fem_trig_trfrmdiff_vs_trfrm");
      key_fem_evfrm_vs_trfrm->Write("key_fem_evfrm_vs_trfrm");

    }

  
    return true;
  }
}
#endif

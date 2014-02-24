#ifndef CHARGEANA_CC
#define CHARGEANA_CC

#include "ChargeAna.hh"

namespace larlight {

  bool ChargeAna::initialize() {

    //Define Variables
    _event_num = 0;
    _coll_baseline = 400;
    //Define Histograms
    TotCharge      = new TH1D("TotCharge",      "Tot Charge in Evt.; Charge [ADCs]",        100, 10000,  30000);
    ChargePerWire  = new TH1D("ChargePerWire",  "Charge on Wire; Wire num; Charge [ADCs]", 8000,     0,   8000);
    ChargePerPulse = new TH1D("ChargePerPulse", "Charge on Pulse; Charge[ADCs]",             30,     0,   3000); 
    LowEnPulses    = new TH1D("LowEnPulses",    "Charge on Pulse; Charge[ADCs]",             15,     0,    100); 
    NumHits        = new TH1D("NumHits",        "Number of Hits for Event",                  20,     0,    100);

    return true;
  }
  
  bool ChargeAna::analyze(storage_manager* storage) {

    _event_num += 1;

    //Define Histograms
    /*
    char c[25];
    sprintf(c,"ChargePerWire_%d",_event_num);
    char d[25];
    sprintf(d,"ChargePerPulse_%d",_event_num);
    char e[25];
    sprintf(e,"TotCharge_%d",_event_num);
    char f[25];
    sprintf(f,"NumHits_%d",_event_num);
    TH1D* ChargePerWire  = new TH1D(c,  "Charge on each Wire; wire; ADCs",  8000,    0,  8000);
    TH1D* ChargePerPulse = new TH1D(d, "ADCs on each pulse; ADCs",            30,    0,  3000);
    TH1D* TotCharge      = new TH1D(e, "Tot Charge"           ,              100,    0,100000);
    TH1D* NumHits        = new TH1D(f,      "Number of Hits",                 50,    0,   300);
    */

    //Total Charge on this event
    int tot_ADCs    = 0;
    //Total Charge on individual wire
    int wire_ADCs   = 0;
    int pulse_ADCs  = 0;
    //Total number of hits
    int num_hits    = 0;
    
    const event_fifo *event_wf = (event_fifo*)(storage->get_data(DATA::TPCFIFO));
    
    //waveform counter
    int wfnum = 0;
    //time counted
    int time_counted = 0;

    //Loop over all waveforms
    for (size_t i=0; i<event_wf->size(); i++){
      
      wfnum += 1;
      
      //Check for empty waveforms!
      const fifo* tpc_data = (&(event_wf->at(i)));
      if(tpc_data->size()<1){
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Found 0-length waveform: Event %d ... Ch. %d",event_wf->event_number(),tpc_data->channel_number()));
	continue;
      }

      //reset charge counted if new channel number (new wire)
      if ( i > 0 ){
	if ( (event_wf->at(i)).channel_number() != (event_wf->at(i-1)).channel_number() ){
	  time_counted = 0;
	}
      }

      int wf_time = tpc_data->readout_sample_number_RAW();
      //determine if collection plane
      //(do this better...)
      if ( tpc_data->at(0) < 1500 )
	{

	  bool found_pulse = false;

	  for (int adc_index=0; adc_index<tpc_data->size(); adc_index++)
	    {
	      int adcs = tpc_data->at(adc_index);

	      //find pulse and count ADCs
	      //for now if 2 ADCs above baseline then we found a pulse
	      //AND if not double counted (by looking at time)
	      if ( ((adcs-_coll_baseline) >= 3) )
		{
		  if ( adc_index+wf_time > time_counted )
		    {
		      //update time that has been counted
		      time_counted = adc_index+wf_time;
		      found_pulse = true;
		      tot_ADCs    += (adcs-_coll_baseline);
		      wire_ADCs   += (adcs-_coll_baseline);
		      pulse_ADCs  += (adcs-_coll_baseline);
		    }
		}
	      else //not a pulse...if was before write new pulse
		{
		  if (found_pulse) {
		    ChargePerPulse->Fill(pulse_ADCs);
		    if ( pulse_ADCs < 100 ) { LowEnPulses->Fill(pulse_ADCs); }
		    num_hits+=1;
		    pulse_ADCs = 0;
		  }
		  found_pulse = false;
		}

	    }//for all bins in this waveform
	  
	  ChargePerWire->AddBinContent(tpc_data->channel_number(),wire_ADCs);
	  wire_ADCs = 0;

	}//if collection plane

    }//loop over all waveforms in event      

    std::cout << "Num Hits: " << num_hits << std::endl;
    NumHits->Fill(num_hits);
    num_hits = 0;

    TotCharge->Fill(tot_ADCs);    
    tot_ADCs = 0;

    return true;
  }

  bool ChargeAna::finalize() {

    NumHits->Write();
    TotCharge->Write();
    ChargePerPulse->Write();
    LowEnPulses->Write();
    ChargePerWire->Write();
    return true;
  }
}
#endif

#ifndef CHARGEANA_CC
#define CHARGEANA_CC

#include "ChargeAna.hh"

namespace larlight {

  bool ChargeAna::initialize() {

    //Define Variables
    _event_num = 0;
    _coll_baseline = 400;
    
    //Define Histograms
    TotCharge      = new TH1D("TotCharge",      "Total Charge",                            100,     0,   100000);
    ChargePerWire  = new TH1D("ChargePerWire",  "Charge on each Wire; wire; ADCs",        8000,  4000,     6000);
    ChargePerPulse = new TH1D("ChargePerPulse", "ADCs on each pulse; ADCs",                100,     0,     2000);

    return true;
  }
  
  bool ChargeAna::analyze(storage_manager* storage) {

    _event_num += 1;

    //Total Charge on this event
    int tot_ADCs    = 0;
    //Total Charge on individual wire
    int wire_ADCs   = 0;
    int pulse_ADCs  = 0;
    
    const event_fifo *event_wf = (event_fifo*)(storage->get_data(DATA::TPCFIFO));
    
    //waveform counter
    int wfnum = 0;

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

      //determine if collection plane
      //(do this better...)
      if ( tpc_data->at(0) < 2000 )
	{

	  bool found_pulse = false;

	  for (Int_t adc_index=0; adc_index<tpc_data->size(); adc_index++)
	    {

	      int adcs = tpc_data->at(adc_index);

	      //find pulse and count ADCs
	      //for now if 2 ADCs above baseline then we found a pulse
	      if ( (adcs-_coll_baseline) >= 2 )
		{
		  found_pulse = true;
		  tot_ADCs    += (adcs-_coll_baseline);
		  wire_ADCs   += (adcs-_coll_baseline);
		  pulse_ADCs  += (adcs-_coll_baseline);
		}
	      else //not a pulse...if was before write new pulse
		{
		  if (found_pulse) { ChargePerPulse->Fill(pulse_ADCs); pulse_ADCs = 0; }
		  found_pulse = false;
		}

	    }//for all bins in this waveform
	  
	  ChargePerWire->AddBinContent(tpc_data->channel_number(),wire_ADCs);
	  wire_ADCs = 0;

	}//if collection plane

    }//loop over all waveforms in event      
    
    TotCharge->Fill(tot_ADCs);    
  
    return true;
  }

  bool ChargeAna::finalize() {

    TotCharge->Write();
    ChargePerWire->Write();
    ChargePerPulse->Write();
  
    return true;
  }
}
#endif

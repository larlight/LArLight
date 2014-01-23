#ifndef DAVID_TIMING1_CC
#define DAVID_TIMING1_CC

#include "david_timing1.hh"

namespace larlight {

  bool david_timing1::initialize() {
    
    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you need, you have an output root file pointer "_fout".
    
    pmt_nu_evno = new TH1D("pmt_nu_evno","Event number",100,0,100);

    //Waveform
    hWF = new TH1D("hWF", "Waveform", 1500, 0, 1500);
    //Waveform maxima
    saturation_histo = new TH1D("saturation_histo", "Waveform Saturation Location (Samples)", 30, 0, 30); 
    pulse_start = new TH1D("pulse_start", "Waveform start location (samples)", 30, 0, 30); 
    
    //Start Times
    reco_start = new TH1D("reco_start", "Reco Start Times", 30, 5, 7);
    reco_start_cut = new TH1D("reco_start_cut", "Reco Start Times < 2275 ADC", 30, 5, 7);
    reco_start_fit = new TH1D("reco_start_fit", "Reco Start Times - 3 Points", 30, 5, 7);

    for (Int_t y=0; y<1500; ++y){
      ADCs[y] = 0.0;
    }

    k = 0;

    return true;
    
  }
  
  bool david_timing1::analyze(storage_manager* storage) {
    
    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "data" pointer which contains 
    // event-wise data. For a reference of pmt_wf_collection class instance, 
    // see the class index in the documentation.
    // 
    // Example to print out event id on screen...
    //
    
    const event_fifo *event_wf = (event_fifo*)(storage->get_data(DATA::PMTFIFO));
 
    pmt_nu_evno->Fill(event_wf->event_number());

    //Channel Number
    _nch=0;
    _nrd=0;

    //Where and by how much the channel is saturated
    double saturation = 0.0;
    int saturation_location = 1;
    //Where the channel "begins" seing the pulse
    //For now >10 ADC counts above baseline???
    int pulse_begin = 1;
    int found = 0; //found begin? if yes =1

    //Create temp histogram
    char c[15];
    char d[15];
    strcpy(c,Form("fff%d",event_wf->event_number()));
    TH1D* temphist = new TH1D(c,"pulse",50,0,50); 
    strcpy(d,Form("ggg%d",event_wf->event_number()));
    TH1D* fitslope = new TH1D(d,"fit",50,0,50);
    
   
    //loop over all channels
    for (size_t i=0; i<event_wf->size(); ++i){

      //Check for empty waveforms!
      const fifo* pmt_data = (&(event_wf->at(i)));
      if(pmt_data->size()<1){
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Found 0-length waveform: Event %d ... Ch. %d",event_wf->event_number(),pmt_data->channel_number()));
	continue;
      }

      //Select only Channel 2
      if (pmt_data->channel_number()==2){

	//variables to find start time
	Double_t start_time;
	Double_t x1, x2, y1, y2, x3, y3;
	Int_t found_begin = 0; //Set to 1 if found first count > 4 ADC
	Double_t baseline = 0;
		
	//Figure out baseline
	for (Int_t adc_baseline=0; adc_baseline<3; ++adc_baseline){
	  baseline += pmt_data->at(adc_baseline);
	}
	baseline = baseline/3.0;

	//Get waveform
	for (Int_t adc_index=0; adc_index<50; ++adc_index){
	  
	  //FIND "START" TIME FOR PULSE
	  //1) Find first hit that is >5 ADC above baseline
	  //2) Find slope for 2 points starting from this first one
	  //3) Trace back to baseline. That is the starting time
	  if ((pmt_data->at(adc_index) > (baseline+4)) & (found_begin < 3)){
	    if (found_begin == 0){
	      x1 = adc_index+0.5;  //add 0.5 to center in bin
	      y1 = pmt_data->at(adc_index);
	      found_begin += 1;
	    }
	    else if(found_begin == 1){
	      x2 = adc_index+0.5;  //add 0.5 to center in bin
	      y2 = pmt_data->at(adc_index);
	      found_begin += 1;
	    }
	    else if(found_begin == 2){
	      x3 = adc_index+0.5;  //add 0.5 to center in bin
	      y3 = pmt_data->at(adc_index);
	      found_begin += 1;
	    }
	  }
	    
	  //Keep track of sum of all ADC counts for Ch2 for all events
	  ADCs[adc_index] += ((pmt_data->at(adc_index))-baseline);
	  temphist->SetBinContent(adc_index+1,pmt_data->at(adc_index));

	  //Find where the beam saturates
	  if (pmt_data->at(adc_index) > saturation){
	    saturation = pmt_data->at(adc_index);
	    saturation_location = adc_index;
	  }
	  //Find where the beam begins
	  if ((pmt_data->at(adc_index) > (baseline+10.0)) & (found == 0)){
	    pulse_begin = adc_index;
	    found = 1;
	  }

	}

	//Find Start Time. Steps 2) and 3)
	Double_t slope = (y2-y1)/(x2-x1);
	//y = m*x+b
	//first find b
	Double_t b = y2-slope*x2;
	start_time = (baseline-b)/slope;
	reco_start->Fill(start_time);
	temphist->GetXaxis()->SetTitle(Form("%f",start_time));
	//Apply cut based on first bin ADC value
	if (y1 < 2275){
	  reco_start_cut->Fill(start_time);
	}

	//If 3 points, reconstruct start time
	//create temporary histogram of 3 data points
	if (y3 < 4095){
	  fitslope->SetBinContent(x1+1,y1);
	  fitslope->SetBinContent(x2+1,y2);
	  fitslope->SetBinContent(x3+1,y3);
	  temphist->Fit("pol1","","",x1,x3);
	  TF1 *fitparam = temphist->GetFunction("pol1");
	  //std::cout << r << std::endl;
	  Double_t b2 = fitparam->GetParameter(0);
	  Double_t slope2 = fitparam->GetParameter(1);
	  //find x-value for y = baseline
	  Double_t start_fit = (baseline-b2)/slope2;
	  std::cout << start_fit << std::endl;
	  reco_start_fit->Fill(start_fit);
	}
	else{
	  reco_start_fit->Fill(start_time);
	}
	//Write individual histogram for waveform
	temphist->Write();
	
	
	//Filling histograms with waveform saturation and begin info
	saturation_histo->Fill(saturation_location);
	saturation = 0.0;
	saturation_location = 1;
	pulse_start->Fill(pulse_begin);
	pulse_begin = 1;
	found = 0;

      }
      _nch++;
      _nrd++;
    
    k++;
    }    

    return true;
    
  }
  
  bool david_timing1::finalize() {

     for (Int_t r; r<1500; ++r){
       hWF->SetBinContent(r+1,ADCs[r]);
    }
    
    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    
    if(_fout) {
      _fout->cd();
      
      //output histograms

      pmt_nu_evno->Write();
      hWF->Write();
      saturation_histo->Write();
      pulse_start->Write();
      reco_start->Write();
      reco_start_fit->Write();

    }
    return true;
  }
}
#endif

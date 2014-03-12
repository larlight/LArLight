#ifndef DAVID_TIMING1_CC
#define DAVID_TIMING1_CC

#include "david_timing1.hh"

namespace larlight {

  bool david_timing1::initialize() {
    
    // Do all variable initialization you wish to do here.
    // If you need, you have an output root file pointer "_fout".
    
    pmt_nu_evno =          new TH1D("pmt_nu_evno", "Event number",   100,0,100);
    //Histo of all baselines
    baseline_histo =       new TH1D("baseline_histo", "Baseline",   100, 2040, 2080);
    //Waveform
    hWF =                  new TH1D("hWF","Waveform", 1500, 0, 1500);
    //Waveform maxima
    saturation_histo =     new TH1D("saturation_histo", "Waveform Saturation Location (Samples)", 30, 0, 30); 
    pulse_start =          new TH1D("pulse_start", "Waveform start location (samples)", 30, 0, 30); 
    //Start Times
    reco_start =           new TH1D("reco_start", "Reco Start Times", 50, 0, 10);
    reco_start_fit =       new TH1D("reco_start_fit", "Reco Start Times - 3 Points", 50, 0, 10);

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

    
    //I think here it finds waveforms for each channel
    //So if there are 2 waveforms (i.e. readouts) for channel 2
    //In this loop twice there will be a pmt_data vector
    //with channel 2 as the channel
    //loop over all channels
    
    //Counter to keep track of which waveform for this channel
    //for this event we are looking at
    int waveform_counter = 0;

    //Loop over all waveforms for this event
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

	//Up the counter for the waveforms
	waveform_counter += 1;

	//Create temp histogram
	char c[25];
	char d[25];
	//strcpy(c,Form("fff%d",event_wf->event_number()));
	//Change name of histogram to avoid over-writing
	sprintf(c,"fff_ev_%d_n_%d",event_wf->event_number(),waveform_counter);
	int pulse_size = 200;//pmt_data->size();
	int pulse_init = 0;//pmt_data->readout_sample_number_64MHz();
	int pulse_end = pulse_init+pulse_size;
	TH1D* temphist = new TH1D(c,"pulse",pulse_size,pulse_init,pulse_end); 
	sprintf(d,"ggg_ev_%d_n_%d",event_wf->event_number(),waveform_counter);
	//strcpy(d,Form("ggg%d",event_wf->event_number()));
	TH1D* fitslope = new TH1D(d,"fit",pulse_size,pulse_init,pulse_end);


	//variables to find start time
	Double_t start_time;
	Double_t x1, x2, y1, y2, x3, y3;
	//for now...just something above saturation point 4095
	y3 = 5000;
	Double_t b1, b2, b3;
	Int_t found_begin = 0; //Set to 1 if found first count > 4 ADC
	Double_t baseline = 0;
		
	//Figure out baseline
	//Sometimes wavefunctions do not have many samples after pulse.
	//In that case, use absolute first point as baseline
	if (pmt_data->size() < 300){
	  baseline = pmt_data->at(0);
	}
	else{
	  for (Int_t adc_baseline=200; adc_baseline<300; ++adc_baseline){
	    baseline += pmt_data->at(adc_baseline);
	  }
	  baseline = baseline/100.0;
	  std::cout << baseline << std::endl;
	}
	//Fill Baseline Histo
	baseline_histo->Fill(baseline);


	//Get waveform
	for (Int_t adc_index=0; adc_index<pmt_data->size(); ++adc_index){

	  //FIND "START" TIME FOR PULSE
	  //1) Find first hit that is >5 ADC above baseline
	  //2) Find slope for 2 points starting from this first one
	  //3) Trace back to baseline. That is the starting time
	  if ((pmt_data->at(adc_index) > (baseline+5)) & (found_begin < 3)){
	    if (found_begin == 0){
	      x1 = pulse_init+adc_index+0.5;  //add 0.5 to center in bin
	      b1 = adc_index;
	      std::cout << Form("b1: %f",b1) << std::endl;
	      y1 = pmt_data->at(adc_index);
	      found_begin += 1;
	    }
	    else if(found_begin == 1){
	      x2 = pulse_init+adc_index+0.5;  //add 0.5 to center in bin
	      b2 = adc_index;
	      std::cout << Form("b2: %f",b2) << std::endl;
	      y2 = pmt_data->at(adc_index);
	      found_begin += 1;
	    }
	    else if(found_begin == 2){
	      x3 = pulse_init+adc_index+0.5;  //add 0.5 to center in bin
	      b3 = adc_index;
	      std::cout << Form("b3: %f",b3) << std::endl;
	      y3 = pmt_data->at(adc_index);
	      found_begin += 1;
	    }
	  }//get info on first points of pulse


          //ERROR HERE >>>>
	  //Keep track of sum of all ADC counts for Ch2 for all events
	  //ADCs[adc_index] += ((pmt_data->at(adc_index))-baseline);
	  //std::cout << Form("current bin %d",current_bin) << std::endl;
	  temphist->SetBinContent(adc_index+1,pmt_data->at(adc_index));
	  //std::cout << Form("Filled bin %d",adc_index) << std::endl;

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
	  //ERROR HERE <<<<
	  
	}//loop through all samples


	//Find Start Time. Steps 2) and 3)
	Double_t slope = (y2-y1)/(b2-b1);
	//y = m*x+b
	//first find b
	Double_t b = y2-slope*b2;
	start_time = (baseline-b)/slope;
	//std::cout << start_time << std::endl;
	reco_start->Fill(start_time);
	std::cout<< start_time << std::endl;
	temphist->GetXaxis()->SetTitle(Form("%f",start_time));
	
	//If 3 points, reconstruct start time
	//create temporary histogram of 3 data points
	//TURNED OFF - UNSET FALSE STATEMENT
	if ((y3 < 4095)){
	  //fitslope->SetBinContent(b1+1,y1);
	  //fitslope->SetBinContent(b2+1,y2);
	  //fitslope->SetBinContent(b3+1,y3);
	  temphist->Fit("pol1","","",x1,x3);
	  TF1 *fitparam = temphist->GetFunction("pol1");
	  Double_t b2 = fitparam->GetParameter(0);
	  Double_t slope2 = fitparam->GetParameter(1);
	  Double_t start_fit = ((baseline-b2)/slope2)-pulse_init;
	  std::cout << Form("Reconstructed Start Time: %f",start_fit) << std::endl;
	  //Avoid Seg Fault: If start_fit out of range print error
	  if ((start_fit > 0) & (start_fit < 10)){
	    reco_start_fit->Fill(start_fit);
	  }
	  else{
	    std::cout << "*******\n ERROR: star_fit out of bounds\n *******" << std::endl;
	  }
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
	
      }//if channel 2

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
      baseline_histo->Write();
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

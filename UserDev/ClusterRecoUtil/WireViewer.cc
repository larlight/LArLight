#ifndef WIREVIEWER_CC
#define WIREVIEWER_CC

#include "WireViewer.hh"

namespace cluster{

  WireViewer::WireViewer()
  {
    hWireView = 0;
    fThresholdSigma = 0.001;
    fNSampleSigma   = 6;
    fFilterBaseline = false;
  }

  void WireViewer::GenerateHisto(const larlight::event_wire *ewire, int plane)
  {
    if(!ewire){
      std::cout<<"osfiaowehifaowef"<<std::endl;
      return;
    }
    if(hWireView) delete hWireView;

    const larutil::Geometry* mygeo = larutil::Geometry::GetME();
    const larutil::GeometryUtilities* mygeoutil = larutil::GeometryUtilities::GetME();

    double fTimetoCm = mygeoutil->TimeToCm();
    double fWiretoCm = mygeoutil->WireToCm();

    int min_wire = 99999;
    int max_wire = -99999;
    int min_TDC = 0;
    int max_TDC = -99999;

    //find minimum and maximum channel for histo boundaries
    for(auto const wire : *ewire) {

      if((int)mygeo->ChannelToPlane(wire.Channel()) != plane)
	continue;
      
      if((int)mygeo->ChannelToWire(wire.Channel()) < min_wire)
	min_wire = mygeo->ChannelToWire(wire.Channel());
      if((int)mygeo->ChannelToWire(wire.Channel()) > max_wire)
	max_wire = mygeo->ChannelToWire(wire.Channel());

      //min_TDC tick assumed to start at 0 ... wire waveform doesn't have timing info
      if((int)wire.Signal().size() > max_wire)
	max_TDC = (int)wire.Signal().size();
      
    } //end first loop over event_wire to get histo boundaries
  
    //convert to cm
    min_wire *= fWiretoCm;
    max_wire *= fWiretoCm;
    min_TDC *= fTimetoCm;
    max_TDC *= fTimetoCm;

    hWireView = new TH2D("hWireView",
			 Form("Event %d (Plane %d); Wire [cm]; TDC[cm]",ewire->event_id(),plane),
			 max_wire-min_wire,min_wire-0.5,max_wire-0.5,
			 max_TDC-min_TDC,min_TDC-0.5,max_TDC-0.5);

    //    std::cout<<Form("wire: max,min = (%d,%d), TDC: max,min = (%d,%d)\n",
    //		    max_wire,min_wire,max_TDC,min_TDC);

    //loop again over stuff to fill the histo
    for(auto const wire : *ewire) {
      
      if((int)mygeo->ChannelToPlane(wire.Channel()) != plane)
        continue;

      if(!fFilterBaseline) {
	
	for(size_t i = 0; i < wire.Signal().size(); i++)

	  hWireView->Fill(mygeo->ChannelToWire(wire.Channel()),i,wire.Signal().at(i));
	//	std::cout<<Form("Filled bin (%d, %d) with %f",mygeo->ChannelToWire(wire.Channel()),i,wire.Signal().at(i))<< std::endl;

      }else{

	bool store_switch = false;
	std::pair<size_t,size_t> store_index;
	std::vector<std::pair<size_t,size_t> > store_sections;

	// Find a region of interest
	for(size_t i = 0; i < (wire.Signal().size() - fNSampleSigma); i++) {

	  // compute sigma
	  double sigma_pow2=0;
	  double mean =0;
	  for(size_t j = 0; j < fNSampleSigma; ++j) mean += wire.Signal().at(i+j);
	  mean /= (double)(fNSampleSigma);
	  
	  for(size_t j = 0; j < fNSampleSigma; ++j) sigma_pow2 += pow(wire.Signal().at(i+j) - mean,2);
	  
	  sigma_pow2 /= (double)(fNSampleSigma);

	  if(sigma_pow2 > fThresholdSigma) {

	    if(!store_switch) {

	      if(store_sections.size()) {

		size_t last_end_index = (*store_sections.rbegin()).second;

		store_index.first = ( i > last_end_index ? i : (last_end_index+1) );

	      }
	      else

		store_index.first = i;

	    }

	    store_index.second = (i+fNSampleSigma);

	    store_switch = true;

	  }else if(store_switch){
	    
	    store_sections.push_back(store_index);

	    store_switch = false;

	  }
	  //std::cout<<i<<" : "<<wire.Signal().at(i) << " : " << mean<< std::endl;
	}

	if(store_switch)

	  store_sections.push_back(store_index);

	for(auto const &index_pair : store_sections) {

	  for(size_t i=index_pair.first; i<index_pair.second && i < wire.Signal().size(); ++i)

	    hWireView->Fill(mygeo->ChannelToWire(wire.Channel()),i,wire.Signal().at(i));

	}

      }
      //break;
    }//end loop over event wire to fill histo

  } // end GenerateHisto

}
#endif

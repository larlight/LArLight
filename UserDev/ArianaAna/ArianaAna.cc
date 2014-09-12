#ifndef ARIANAANA_CC
#define ARIANAANA_CC

#include "ArianaAna.hh"

namespace larlight {

  bool ArianaAna::initialize() {

	h1= new TH1D("h1","charge hist; charge; clusters",100,0,1e4);	

    return true;
  }
  
  bool ArianaAna::analyze(storage_manager* storage) {
 

	auto my_cluster_v= (const event_cluster*)(storage->get_data(DATA::FuzzyCluster));
	auto my_hit_v= (const event_hit*)(storage->get_data(DATA::GausHit));
	
	double total_charge 	= 0;

	for(auto const& c : *my_cluster_v){
		double cluster_charge   = 0;
//		std::cout <<Form(" Cluster ID: %d ... Charge: %g", c.ID(), c.Charge()) <<std::endl;
		for(auto const& index : c.association(DATA::GausHit)) 
			cluster_charge+=my_hit_v->at(index).Charge();
		
  		std::cout<<"\nThe charge sum for cluster is: "<<cluster_charge <<std::endl;
  		h1->Fill(cluster_charge);  	  
}

    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "storage" pointer which contains 
    // event-wise data. To see what is available, check the "Manual.pdf":
    //
    // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
    // 
    // Or you can refer to Base/DataFormatConstants.hh for available data type
    // enum values. Here is one example of getting PMT waveform collection.
    //
    // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
    //
    // if( event_fifo )
    //
    //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
    //
  
    return true;
  }

  bool ArianaAna::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
     if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //
  
    return true;
  }
}
#endif

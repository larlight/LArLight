#ifndef TESTEFF_CC
#define TESTEFF_CC

#include "TestEff.hh"
// #include "../ClusterRecoUtil/ClusterParamsAlgNew.hh"


namespace larlight {

  bool TestEff::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    return true;
  }
  
  bool TestEff::analyze(storage_manager* storage) {
  
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
    event_cluster * my_cluster_v = (event_cluster *)(storage->get_data(DATA::FuzzyCluster));
  
    event_hit * my_hit_v = (event_hit*)(storage->get_data(my_cluster_v->get_hit_type()));
   
   
   
    ::cluster::ClusterParamsAlgNew  fCPAlg; // = new cluster::ClusterParamsAlgNew();
 
    for(auto const clustit : *my_cluster_v) {
      std::cout << " Clust ID " << clustit.ID() << std::endl;    
     //auto const hit_index_v = clustit.association(my_cluster_v->get_hit_type());
        auto const hit_index_v = clustit.association(DATA::FFTHit);
     
	for(auto const hit_index : hit_index_v) {
      
		my_hit_v->at(hit_index);
       
	      }
     
	}
  
  
  
    return true;
  }

  bool TestEff::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //
  
    return true;
  }
}
#endif

#ifndef MCCLUSTERSTUDY_CC
#define MCCLUSTERSTUDY_CC

#include "MCClusterStudy.hh"

namespace larlight {

  bool MCClusterStudy::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    _mcclus_tree = 0;
    PrepareTTree();

    //setting variables that i should be getting from geometry service...
    _wire_2_cm = 0.3; 
    _time_2_cm = 0.0818566;

    return true;
  }
  
  bool MCClusterStudy::analyze(storage_manager* storage) {
    


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

    larlight::event_cluster* ev_cluster = (event_cluster*)storage->get_data(DATA::ShowerAngleCluster);
    if(!ev_cluster) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, ShowerAngleCluster!"));
      return false;
    }
    
    larlight::event_mcshower* ev_mcshower = (event_mcshower*)storage->get_data(DATA::MCShower);
    if(!ev_mcshower) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
      return false;
    }

    //for kaleko's current study, this only works for single-shower events
    //for multi shower events, use bryce's code that matches reco clusters with showers.. demand that 90% of the hits in my reco shower came from one MC shower, then use that MC shower for the comparison
    if(ev_mcshower->size() != 1){
      print(larlight::MSG::ERROR,__FUNCTION__,Form("This event has more than one MCShower in it! In fact, it has %d! Wtf?",(int)ev_mcshower->size()));
      return false;
    }
    
    
    const larutil::GeometryUtilities *myGeoUtil = larutil::GeometryUtilities::GetME();


    //assuming one true shower, this will hold the PxPoint of the mother start position
    //one entry for each view
    std::vector<larutil::PxPoint> mother_sw_view(3,larutil::PxPoint());
    
    //loop over MCShowers in the event (there should only be one, for now)
    for(auto i_mcshower: *ev_mcshower){
      
      mother_sw_view.clear();

      //check to make sure i'm choosing the right 3 out of the 4 vector
      Double_t pos[3]={0.};
      //pos[0]=i_mcshower.MotherPosition()[0];
      //pos[1]=i_mcshower.MotherPosition()[1];
      //pos[2]=i_mcshower.MotherPosition()[2];
      pos[0] = 50.;  // x= 50cm
      pos[1] = 0.;    // y= 0cm
      pos[2] = 100.; // z= 100cm

      for(size_t i = 0; i < mother_sw_view.size(); ++i)
	mother_sw_view.push_back(myGeoUtil->Get2DPointProjection(pos,i));
      
      std::cout<<"This MCShower's wire, time shit on view 0 is "
	       <<mother_sw_view[0].w<<", "<<mother_sw_view[0].t<<std::endl;
      std::cout<<"This MCShower's wire, time shit on view 1 is "
	       <<mother_sw_view[1].w<<", "<<mother_sw_view[1].t<<std::endl;
      std::cout<<"This MCShower's wire, time shit on view 2 is "
	       <<mother_sw_view[2].w<<", "<<mother_sw_view[2].t<<std::endl;
         
    }

    
    //loop over reconstructed clusters in the event
    for(auto const i_cluster: *ev_cluster){

      int iview = (int)i_cluster.View();
            
      std::cout<<"this cluster's start position in wire,time is "<<i_cluster.StartPos()[0]
      	       <<", "<<i_cluster.StartPos()[1]<<"."<<std::endl;
      std::cout<<"this cluster is on view "<<iview<<std::endl;
      
      double dw1, dt1, da1, db1, dw2, dt2, da2, db2;
      //calculate start/end refining figure of merit for this cluster
      dw1 = mother_sw_view[iview].w - i_cluster.StartPos()[0];
      dt1 = mother_sw_view[iview].t - i_cluster.StartPos()[1];

      da1 = dw1 * _wire_2_cm;
      db1 = dt1 * _time_2_cm;

      dw2 = mother_sw_view[iview].w - i_cluster.EndPos()[0];
      dt2 = mother_sw_view[iview].t - i_cluster.EndPos()[1];

      da2 = dw2 * _wire_2_cm;
      db2 = dt2 * _time_2_cm;

      _refinestartend_FOM = std::sqrt( da1*da1 + db1*db1 ) - std::sqrt( da2*da2 + db2*db2 );

      //fill TTree
      if(_mcclus_tree) _mcclus_tree->Fill();

    }
     
    
    return true;
  }

  bool MCClusterStudy::finalize() {

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
  
    if(_fout) {

      _fout->cd();
      if(_mcclus_tree)
	_mcclus_tree->Write();

    }
    else 
      print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");

    return true;
  }


  void MCClusterStudy::PrepareTTree() {
    if(!_mcclus_tree) {
      _mcclus_tree = new TTree("mcclus_tree","");
      _mcclus_tree->Branch("refinestartend_FOM", &_refinestartend_FOM, "refinestartend_FOM/D");
    }


  }


 
}







#endif

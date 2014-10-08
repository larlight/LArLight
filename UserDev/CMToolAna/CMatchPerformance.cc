#ifndef CMATCHPERFORMANCE_CC
#define CMATCHPERFORMANCE_CC

#include "CMatchPerformance.hh"

namespace larlight {

  CMatchPerformance::CMatchPerformance() : fMgr(nullptr)
  { 
    _name="CMatchPerformance";
    _fout=0; 
    hMatchQEff=0;
    hMatchQEffEvent=0;
    _cluster_type = DATA::Cluster;
    auto geom = ::larutil::Geometry::GetME();
    fMgr = new ::cmtool::CMatchManager(geom->Nplanes());
  };


  bool CMatchPerformance::initialize() {

    print(MSG::DEBUG,Form("CMatchPerformance::%s",__FUNCTION__),"called...");

    hMatchQEff = new TH1D("hMatchChargeEff",
			  "MC Matched Pair Efficiency; Efficiency; Num. MCShower;",
			  101,-0.005,1.005);

    hMatchQEffEvent = new TH1D("hMatchChargeEffEvent",
			       "MC Matched Pair Efficiency; Efficiency; Num. MCShower;",
			       101,-0.005,1.005);

    return true;


  }
  
  bool CMatchPerformance::analyze(storage_manager* storage) {

    if(this->get_verbosity()==MSG::DEBUG) {

      std::cout<<std::endl;
      print(MSG::DEBUG,Form("CMatchPerformance::%s",__FUNCTION__),"start...");

    }

    //
    // Run CMatchManager
    //
    // (1) Convert larlight "cluster" to a set of PxHit (fmwk independent "cluster")
    // (2) Pass it onto CMatchManager and process
    //

    std::vector<std::vector<larutil::PxHit> > clusters;

    fWatch.Start();

    fHelper.GeneratePxHit(storage,_cluster_type,clusters);
    
    if(this->get_verbosity()==MSG::DEBUG)

      print(MSG::DEBUG,Form("CMatchPerformance::%s",__FUNCTION__),
	    Form("Time to convert Hit=>PxHit for all clusters: %g",fWatch.RealTime()));

    fWatch.Start();

    fMgr->Reset();

    fMgr->SetClusters(clusters);

    try{
      fMgr->Process();
    }catch( ::cmtool::CMTException &e){
      e.what();
      return false;
    }

    if(this->get_verbosity()==MSG::DEBUG)

      print(MSG::DEBUG,Form("CMatchPerformance::%s",__FUNCTION__),
	    Form("Time to process matching: %g",fWatch.RealTime()));

    // Done matching clusters. Get result
    auto bk = fMgr->GetBookKeeper();

    std::vector<std::vector<unsigned int> > reco_match_v;
    bk.PassResult(reco_match_v);

    //std::vector<std::vector<float> > reco_match_charge_v;
    //reco_match_charge_v.resize(reco_match_v.size(),std::vector<float>(geo->Nplanes(),0));

    //
    // Prepare back-tracking using McshowerLookback
    //
    // (1) Load MCShower and SimCh data products
    // (2) Convert them to MCShower/SimCh map necessary for back-tracking
    //

    // Load MCShower and SimCh
    auto ev_mcshower = (const event_mcshower*)(storage->get_data(DATA::MCShower));

    auto ev_simch    = (const event_simch*)(storage->get_data(DATA::SimChannel));

    if(!ev_mcshower || !ev_simch) {

      print(MSG::ERROR,__FUNCTION__,"MCShower/SimCh data product not found!");

      return false;

    }

    if(this->get_verbosity()==MSG::DEBUG) 
      print(MSG::DEBUG,Form("CMatchPerformance::%s",__FUNCTION__),
	    Form("Event %d found %zu matches...",ev_mcshower->event_id(),reco_match_v.size()));
    
    if(!(reco_match_v.size())) return true;

    // Prepare MC<=>Reco matching algorithm
    fAlg.Prepare(storage,_cluster_type);

    //
    // Evaluate efficiency
    //
    double max_correctness=0;
    fWatch.Start();
    for(auto const& match : reco_match_v) {

      double correctness = -1;
      size_t best_match_mcs = 0;
      fAlg.Match(match,best_match_mcs,correctness);

      if(this->get_verbosity() <= MSG::DEBUG) 
	
	print(MSG::DEBUG,Form("CMatchPerformance::%s",__FUNCTION__),Form("Matching efficiency: %g",correctness));
      
      hMatchQEff->Fill(correctness);
      if(max_correctness < correctness) max_correctness = correctness;
    }

    if(this->get_verbosity()==MSG::DEBUG)

      print(MSG::DEBUG,Form("CMatchPerformance::%s",__FUNCTION__),
	    Form("Time to evaluate every matched pair: %g",fWatch.RealTime()));
    
    if(reco_match_v.size()) {

      if(this->get_verbosity() <= MSG::DEBUG) 

	print(MSG::DEBUG,Form("CMatchPerformance::%s",__FUNCTION__),Form("MAX efficiency in this event: %g",max_correctness));

      hMatchQEffEvent->Fill(max_correctness);

    }

    return true;
  }

  bool CMatchPerformance::finalize() {

    if(this->get_verbosity()==MSG::DEBUG)
      
      print(MSG::DEBUG,Form("CMatchPerformance::%s",__FUNCTION__),"called...");

    if(_fout) {

      _fout->cd();
      
      hMatchQEff->Write();

      hMatchQEffEvent->Write();

    }

    return true;
  }

}
#endif

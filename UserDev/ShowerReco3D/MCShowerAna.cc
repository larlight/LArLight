#ifndef MCSHOWERANA_CC
#define MCSHOWERANA_CC

#include "MCShowerAna.hh"

namespace larlight {

  MCShowerAna::MCShowerAna() : ana_base()
  {
    _name = "MCShowerAna";

    hMCX = hMCY = hMCZ = 0;
    hdcosX = hdcosY = hdcosZ = 0;
    
    hEner = hMCEner = recohEner = 0;

    hMCMotherEner = 0;

    hdEdx = 0;

  }

  bool MCShowerAna::initialize() {

    hMCX = new TH1D("hMCX","",100,-120,120);
    hMCY = new TH1D("hMCY","",100,-120,120);
    hMCZ = new TH1D("hMCZ","",100,-120,120);
    
    hdcosX = new TH1D("hdcosX","",100,-5,5);
    hdcosY = new TH1D("hdcosY","",100,-5,5);
    hdcosZ = new TH1D("hdcosZ","",100,-5,5);
    
    hEner = new TH1D("hEner","",100,-1000,1000);
    hMCEner = new TH1D("hMCEner","Deposited MC Shower Energy; MeV; MCShower",100,-10,2000);
    hMCMotherEner = new TH1D("hMCMotherEner","Mother Energy; MeV; MCShower",100,-10,2000);
    recohEner = new TH1D("recohEner","",100,-10,2000);
    
    hdEdx = new TH1D("hdEdx","",60,0,12);

    return true;
  }
  
  bool MCShowerAna::analyze(storage_manager* storage) {

    auto geo  = ::larutil::Geometry::GetME();
    auto detp = ::larutil::DetectorProperties::GetME();

    // Get data products from storage
    auto mcshower_v = (event_mcshower* )( storage->get_data(DATA::MCShower) );
    auto shower_v   = (event_shower*   )( storage->get_data(DATA::Shower)   );
    auto cluster_v  = (event_cluster*  )( storage->get_data(DATA::Cluster)  );
    auto hit_v      = (event_hit*      )( storage->get_data(DATA::GausHit)  );
    
    // Check data exists
    if( !mcshower_v || !shower_v || !cluster_v || !hit_v) {
      std::cerr<<"Missing some data! not doing anything..."<<std::endl;
      return false;
    }

    // Here, we assume there is only 1 MCShower exists in an event
    // because otherwise we have to think about which MCShower possibly corresponds to 
    // a given reconstructed shower.
    if( mcshower_v->size() != 1) {
      std::cerr<<"Found > 1 MCShower! Ignore this event... "<<std::endl;
      return false;
    }

    //
    // Use LArLight's version of BackTracker
    //
    if(!fBTAlg.Prepare(storage)) return false;

    auto mcshower_index = fBTAlg.ShowerIDs().at(0);

    auto const& mcs = mcshower_v->at(mcshower_index);
    auto const& mc_mom_orig = mcs.MotherMomentum();
    auto const& mc_mom = mcs.DaughterMomentum();
    auto const& mc_vtx = mcs.DaughterPosition();
    std::vector<double> mc_dcos(3,0);
    for(size_t i=0; i<3; ++i)
      mc_dcos.at(i) = mc_mom.at(i)/mc_mom.at(3);

    // Loop over reco-ed shower and loop over corresponding clusters & hits 
    // to fill MC charge information
    for(auto const& reco_shower : *shower_v) {
      
      auto& cluster_index = reco_shower.association(DATA::Cluster);

      std::vector<double> recoq(3,0);        // Reconstructed charge on a given plane
      std::vector<double> mcq_total(3,0);    // Total MCShower's charge on a given plane
      std::vector<double> mcq_detected(3,0); // Detected MCShower's charge 
      
      // Loop over associated clusters to fill mc/reco charge info 
      for(auto const& c_index : cluster_index) {

	auto& hit_index = cluster_v->at(c_index).association(DATA::GausHit);
	UChar_t plane = geo->ChannelToPlane(hit_v->at(hit_index.at(0)).Channel());

	std::vector<const larlight::hit*> hits;
	hits.reserve(hit_index.size());
	for(auto const& h_index : hit_index) {
	  hits.push_back(&(hit_v->at(h_index)));
	  recoq.at(plane) += hit_v->at(h_index).Charge();
	}
	mcq_total.at(plane)    = mcs.Charge(GEO::View_t(plane));
	mcq_detected.at(plane) = fBTAlg.MCShowerQ(hits).at(0) * detp->ElectronsToADC();
      }

      for(size_t i=0; i<3; ++i) {
	std::cout << "Plane " << i << std::endl
		  << "Reco-ed Q: " << recoq.at(i) << std::endl
		  << "MC Q: " << mcq_total.at(i) << std::endl
		  << "MC Q in this cluster: " << mcq_detected.at(i) << std::endl
		  << std::endl;
      }

      // 
      // Fill histograms
      //
      auto const& reco_vtx  = reco_shower.ShowerStart();
      auto const& reco_dcos = reco_shower.Direction();
      auto const& reco_en   = reco_shower.Energy();
     
      hdEdx->Fill(reco_shower.dEdx().at(2));
      hMCX->Fill(mc_vtx.at(0) - reco_vtx[0]);
      hMCY->Fill(mc_vtx.at(1) - reco_vtx[1]);
      hMCZ->Fill(mc_vtx.at(2) - reco_vtx[2]);

      hdcosX->Fill(mc_dcos.at(0) - reco_dcos[0]);
      hdcosY->Fill(mc_dcos.at(1) - reco_dcos[1]);
      hdcosZ->Fill(mc_dcos.at(2) - reco_dcos[2]);

      hEner->Fill(mc_mom.at(3) - reco_en.at(2));
      hMCEner->Fill(mc_mom.at(3));
      recohEner->Fill(reco_en.at(2));
      hMCMotherEner->Fill(mc_mom.at(3));
      std::cout
	<< "MCShower Mother Energy    : " << mc_mom_orig.at(3)*1.e3  << std::endl
	<< "MCShower Deposited Energy : " << mc_mom.at(3)            << std::endl
	<< "Reco-ed Energy            : " << reco_en.at(2)           << std::endl
	<< std::endl;
	
    }
    
    return true;
  }

  bool MCShowerAna::finalize() {

    if(_fout) {
      hMCX->Write();
      hMCY->Write();
      hMCZ->Write();
    
      hdcosX->Write();
      hdcosY->Write();
      hdcosZ->Write();
    
      hEner->Write();
      hMCEner->Write();
      hMCMotherEner->Write();
      recohEner->Write();
    
      hdEdx->Write();
    }    
    return true;
  }

}
#endif

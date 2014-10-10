#ifndef COMPUTEPI0MASS_CC
#define COMPUTEPI0MASS_CC

#include "ComputePi0Mass.hh"

namespace larlight {
  
  ComputePi0Mass* ComputePi0Mass::me = 0;

  bool ComputePi0Mass::initialize() {

    hPi0MassPeak = new TH1D("hPi0MassPeak","Pi0 Mass Peak in MeV",100,0,300);

    hEnergyCorr_MomToDaughter = new TH1D("hEnergyCorr_MomToDaughter","Correction: hEnergyCorr_MomToDaughter",100,0.9,1.5);

    hElectronCorr_DepToDet = new TH1D("hElectronCorr_DepToDet","Correction: hElectronCorr_DepToDet",100,0,4);

    return true;
  }
  
  bool ComputePi0Mass::analyze(storage_manager* storage) {

    _mass = -99999.;

    // Load the Showers... need to run shower reconstruction first!
    auto ev_shower  = (const event_shower*)(storage->get_data(_shower_type));
    auto ev_cluster = (const event_cluster*)(storage->get_data(_cluster_type));

    if(!ev_shower ) {
      print(MSG::ERROR,__FUNCTION__,Form("Data product \"%s\" not found...",
					 DATA::DATA_TREE_NAME[_shower_type].c_str()));
      return false;
    }else if(ev_shower->size()<1) {
      print(MSG::ERROR,__FUNCTION__,Form("There are 0 showers in this event! Skipping......"));      
      return false;
    }
    if(!ev_cluster ) {
      print(MSG::ERROR,__FUNCTION__,Form("Data product \"%s\" not found...",
					 DATA::DATA_TREE_NAME[_cluster_type].c_str()));
      return false;
    }else if(ev_cluster->size()<1) {
      print(MSG::ERROR,__FUNCTION__,Form("There are 0 clusters in this event! Skipping......"));      
      return false;
    }else if(!(ev_shower->at(0).association(_cluster_type).size())){
      print(MSG::ERROR,__FUNCTION__,Form("Association %s => %s not found! Skipping...",
					 DATA::DATA_TREE_NAME[_cluster_type].c_str(),
					 DATA::DATA_TREE_NAME[_shower_type].c_str()));
      return false;
    }

    //skip event if !2 showers found.
    if(ev_shower->size() != 2) return true;

    fEnergyCorr_MomToDaughter.resize(ev_shower->size(),1);
    fElectronCorr_DepToDet.resize(ev_shower->size(),1);
    fChargeCorr_DetToPeak.resize(ev_shower->size(),1);

    if(_applyEnergyCorrection)
      ComputeEnergyCorrection(storage);
    /*
    std::cout 
      << std::endl
      << fEnergyCorr_MomToDaughter.at(0)<<" : "<<fElectronCorr_DepToDet.at(0) << std::endl
      << fEnergyCorr_MomToDaughter.at(1)<<" : "<<fElectronCorr_DepToDet.at(1) << std::endl;
    */
    
    //    _mass = Pi0MassFormula3D( ev_shower->at(0).MIPEnergy().at(2) 
    //    				   * fEnergyCorr_MomToDaughter.at(0)
    //    				   * fElectronCorr_DepToDet.at(0),
    //    				   //* fChargeCorr_DetToPeak.at(0),
    //    				   ev_shower->at(1).MIPEnergy().at(2) 
    //    				   * fEnergyCorr_MomToDaughter.at(1)
    //    				   * fElectronCorr_DepToDet.at(1),
    //    				   //* fChargeCorr_DetToPeak.at(1),
    //    				   ev_shower->at(0).Direction(),
    //    				   ev_shower->at(1).Direction());
    

    //    _mass = Pi0MassFormula3D( ev_shower->at(0).MIPEnergy().at(2) 
    //			      * 1.302 + 50,
    //			      ev_shower->at(1).MIPEnergy().at(2)
    //			      * 1.302 + 50,
    //			      ev_shower->at(0).Direction(),
    //			      ev_shower->at(1).Direction());

    
    size_t best_cluster1 = 0;
    for(auto const& index : ev_shower->at(0).association(_cluster_type)) {
      if(ev_shower->at(0).best_plane() == ev_cluster->at(index).View()) break;
      ++best_cluster1;
    }

    size_t best_cluster2 = 0;
    for(auto const& index : ev_shower->at(1).association(_cluster_type)) {
      if(ev_shower->at(1).best_plane() == ev_cluster->at(index).View()) break;
      ++best_cluster2;
    }
    
    _mass = Pi0MassFormula3D( fSECaloAlg.ShowerEnergy(ev_shower->at(0).Energy().at(best_cluster1),::showerreco::energy::kDEFAULT).first,
			      fSECaloAlg.ShowerEnergy(ev_shower->at(1).Energy().at(best_cluster2)).first,
    			      ev_shower->at(0).Direction(),
    			      ev_shower->at(1).Direction());
    
    std::cout<<"in compute thing, mass is "<<_mass<<std::endl;
    hPi0MassPeak->Fill(_mass);
    for(int i = 0; i<2; ++i){
      hEnergyCorr_MomToDaughter->Fill(fEnergyCorr_MomToDaughter.at(i));
      hElectronCorr_DepToDet->Fill(fElectronCorr_DepToDet.at(i));
    }
     

    return true;
  }

  void ComputePi0Mass::ComputeEnergyCorrection(storage_manager* storage)
  {
    
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
      return;
    }

    // Here, we assume there is only 1 MCShower exists in an event
    // because otherwise we have to think about which MCShower possibly corresponds to 
    // a given reconstructed shower.
    //if( mcshower_v->size() != 1) {
    //std::cerr<<Form("Found %zu MCShower! (>1) Ignore this event... ",mcshower_v->size())<<std::endl;
    //return;
    //}

    //
    // Use LArLight's version of BackTracker
    //
    if(!fBTAlg.Prepare(storage)) return;

    std::vector<double> mcs_mother_energy(mcshower_v->size(),0);
    std::vector<double> mcs_daughter_energy(mcshower_v->size(),0);
    std::vector<double> mcs_deposit_charge(mcshower_v->size(),0);
    std::vector<double> mcs_detected_charge(mcshower_v->size(),0);
    for(size_t i=0; i<mcshower_v->size(); ++i) {
      
      mcs_mother_energy.at(i) = mcshower_v->at(i).MotherMomentum().at(3)*1.e3;
      mcs_daughter_energy.at(i) = mcshower_v->at(i).DaughterMomentum().at(3);
      mcs_deposit_charge.at(i)  = mcshower_v->at(i).Charge(GEO::View_t(2));
      
    }

    std::vector<std::vector<double> > shower_mcq_per_mcs;
    shower_mcq_per_mcs.reserve(shower_v->size());
    std::vector<double> shower_mcq_total;
    shower_mcq_total.reserve(shower_v->size());

    for(auto const& reco_shower : *shower_v) {
      
      auto& cluster_index = reco_shower.association(DATA::Cluster);

      std::vector<double> cluster_mcq_per_mcs(mcshower_v->size(),0);

      // Loop over associated clusters to fill mc/reco charge info 
      for(auto const& c_index : cluster_index) {

	auto& hit_index = cluster_v->at(c_index).association(DATA::GausHit);
	UChar_t plane = geo->ChannelToPlane(hit_v->at(hit_index.at(0)).Channel());
	if(plane != 2) continue;

	std::vector<const larlight::hit*> hits;
	hits.reserve(hit_index.size());
	for(auto const& h_index : hit_index)
	  hits.push_back(&(hit_v->at(h_index)));
	//recoq.at(plane) += hit_v->at(h_index).Charge(true);

	auto const& recoq_per_mcs = fBTAlg.MCShowerQ(hits);
	double mcq_sum = 0;
	for(size_t i=0; i<recoq_per_mcs.size(); ++i) {
	  mcs_detected_charge.at(i) += recoq_per_mcs.at(i) * detp->ElectronsToADC();
	  cluster_mcq_per_mcs.at(i)     = recoq_per_mcs.at(i) * detp->ElectronsToADC();
	  mcq_sum += recoq_per_mcs.at(i) * detp->ElectronsToADC();
	}
	shower_mcq_total.push_back(mcq_sum);
	shower_mcq_per_mcs.push_back(cluster_mcq_per_mcs);
      }
    }

    std::vector<double> fQCorrPerMCS(mcshower_v->size(),0);
    for(size_t i=0; i<fQCorrPerMCS.size(); ++i) 

      fQCorrPerMCS.at(i) = mcs_deposit_charge.at(i) / mcs_detected_charge.at(i);

    fEnergyCorr_MomToDaughter.resize(shower_v->size(),1);
    fElectronCorr_DepToDet.resize(shower_v->size(),1);
    //fChargeCorr_DetToPeak.resize(ev_shower->size(),1);

    // Loop over shower
    for(size_t i=0; i<shower_mcq_per_mcs.size(); ++i) {

      double electron_factor = 0;
      double energy_factor   = 0;
      // Loop over MCShower
      for(size_t j=0; j<shower_mcq_per_mcs.at(i).size(); ++j) {
	
	electron_factor += shower_mcq_per_mcs.at(i).at(j) / shower_mcq_total.at(i) * fQCorrPerMCS.at(j);
	energy_factor += shower_mcq_per_mcs.at(i).at(j) / shower_mcq_total.at(i) * mcs_mother_energy.at(j) / mcs_daughter_energy.at(j);
	
      }
      /*
      std::cout<<electron_factor<<std::endl;
      std::cout<<energy_factor<<std::endl;
      */
      fElectronCorr_DepToDet.at(i) = electron_factor;
      fEnergyCorr_MomToDaughter.at(i) = energy_factor;
    }
    
    
  }

  bool ComputePi0Mass::finalize() {

    if(_fout) { 
      _fout->cd(); 
      hPi0MassPeak->Write();
      hEnergyCorr_MomToDaughter->Write();
      hElectronCorr_DepToDet->Write();

      delete hPi0MassPeak;
      delete hEnergyCorr_MomToDaughter;
      delete hElectronCorr_DepToDet;
    }
    else 
      print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    
  
    return true;
  }


  //Get PI0 Mass from photon directions and energy
  float ComputePi0Mass::Pi0MassFormula3D( float Energy1, float Energy2, TVector3 Direction3D_1, TVector3 Direction3D_2){
    
    float angle_3D = acos( Direction3D_1 * Direction3D_2 );

    //using a formula from 
    //http://www.hep.princeton.edu/~mcdonald/examples/piondecay.pdf      
    return pow(4. * Energy1 * Energy2 * pow(sin( 0.5 * angle_3D ),2) , 0.5);

  }

}
#endif

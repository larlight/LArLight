#ifndef COMPUTEPI0MASS_RG_CC
#define COMPUTEPI0MASS_RG_CC

#include "ComputePi0Mass_RG.hh"

namespace larlight {
  
  ComputePi0Mass_RG* ComputePi0Mass_RG::me = 0;

  bool ComputePi0Mass_RG::initialize() {

    _shower_type = DATA::Shower;
    _cluster_type = DATA::RyanCluster;

    hPi0MassPeak = new TH1D("hPi0MassPeak","Pi0 Mass Peak in MeV",50,0,500);
    hPi0MassPeakPull = new TH1D("hPi0MassPeakPull","Pi0 Mass Pull in MeV",25,-250,250);

    hPi0CosCM = new TH1D("hPi0CosCM","Pi0 Cos CM",22,-0.1,1.1);

    hPi0MassPeak_GoodEnergy = new TH1D("hPi0MassPeakGoodEnergy","Pi0 Mass Peak in MeV",50,0,500);
    hPi0MassPeak_GoodAngle = new TH1D("hPi0MassPeakGoodAngle","Pi0 Mass Peak in MeV",50,0,500);
    hPi0MassPeak_GoodAnglePull = new TH1D("hPi0MassPeakGoodAnglePull","Pi0 Mass Pull GoodAngle",50,-200,200);

    hPi0MassPeak_TrueDetector = new TH1D("hPi0MassPeakTrueDetector","Pi0 Mass Peak From True Detector in MeV",50,0,500);




    hPhotondos = new TH2D("hPhotondos","Photondos",50,0,1,50,0,1);
    hOpeningAngle = new TH2D("hPhotonOpening","PhotonAngle",50,0,180,50,0,180);

    hPi0MassPeakdoscut = new TH1D("hPi0MassPeakdoscut","Pi0 Mass Peak in MeV",50,0,500);


    hEnergyCorr_MomToDaughter = new TH1D("hEnergyCorr_MomToDaughter","Correction: hEnergyCorr_MomToDaughter",100,0.9,1.5);

    hElectronCorr_DepToDet = new TH1D("hElectronCorr_DepToDet","Correction: hElectronCorr_DepToDet",100,0,4);

    return true;
  }
  
  bool ComputePi0Mass_RG::analyze(storage_manager* storage) {

    _mass = -99999.;
    _mass_goodEnergy = -99999.;

    // Load the Showers... need to run shower reconstruction first!
    auto ev_shower = (const event_shower*)(storage->get_data(_shower_type));
    auto mc_shower = (const event_mcshower*)(storage->get_data(DATA::MCShower));
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
    
    _mass = Pi0MassFormula3D( ev_shower->at(0).MIPEnergy().at(best_cluster1), 
    			      ev_shower->at(1).MIPEnergy().at(best_cluster2),
    			      ev_shower->at(0).Direction(),
    			      ev_shower->at(1).Direction());

    
    std::cout<<"in compute thing, mass is "<<_mass<<std::endl;
	//double phi = -999;
//	mc_shower->at(0).MotherAngle3D(phi,theta);
	//double theta = -999;
	//mc_shower->at(0).MotherAngle3D(phi,theta);
	//double_t E1 = mc_shower->at(0).MotherMomentum();
	//double_t E2 = mc_shower->at(1).MotherMomentum();
	//std::cout<<" Something from MC shower \n\t E1: "<<E1<<"\n\t E2: "<<E2<<std::endl;
	//std::cout<<" Something from MC shower \n\t Phi: "<<phi<<"\n\t Theta: "<<theta<<std::endl;
	
  //  hPi0MassPeak->Fill(_mass);
    hPi0MassPeak->Fill(3.6*_mass);
    hPi0MassPeakPull->Fill(3.6*_mass -135);
    for(int i = 0; i<2; ++i){
      hEnergyCorr_MomToDaughter->Fill(fEnergyCorr_MomToDaughter.at(i));
      hElectronCorr_DepToDet->Fill(fElectronCorr_DepToDet.at(i));
    }


	// look at how showers reconstruct based on using energy or using angular reco

/*
std::vector<float> mcs_mother_energy(mc_shower->size(),0);
    std::vector<TVector3> mcs_mother_direction(mc_shower->size());
	
    for(size_t i=0; i<mc_shower->size(); ++i) {
      mcs_mother_energy.at(i) = mc_shower->at(i).MotherMomentum().at(3);
		//make a tvector to fit into the mass
      TVector3 v1;	
      v1.SetX(mc_shower->at(i).MotherMomentum().at(0)/mc_shower->at(i).MotherMomentum().at(3));       
      v1.SetY(mc_shower->at(i).MotherMomentum().at(1)/mc_shower->at(i).MotherMomentum().at(3));       
      v1.SetZ(mc_shower->at(i).MotherMomentum().at(2)/mc_shower->at(i).MotherMomentum().at(3));       
      mcs_mother_direction.at(i) = v1;

	}// loop over the mc_shower

	if(mcs_mother_energy.size()==2){
	// looking at how well we do just on getting the angle correct. 
	// here I will just use the corected values of E1 and E2
	    _mass_goodEnergy = Pi0MassFormula3D( mcs_mother_energy[0],
    			      mcs_mother_energy[1],
    			      ev_shower->at(0).Direction(),
    			      ev_shower->at(1).Direction());
	
        _mass_goodAngle = Pi0MassFormula3D(ev_shower->at(0).MIPEnergy().at(0),
    			      ev_shower->at(1).MIPEnergy().at(0),
    			      mcs_mother_direction[0],
    			      mcs_mother_direction[1]);
 */   
    std::vector<float> mcs_Daughter_energy(mc_shower->size(),0);
    std::vector<TVector3> mcs_Daughter_direction(mc_shower->size());
	
    for(size_t i=0; i<mc_shower->size(); ++i) {
      mcs_Daughter_energy.at(i) = mc_shower->at(i).DaughterMomentum().at(3);
		//make a tvector to fit into the mass
      TVector3 v1;	
      v1.SetX(mc_shower->at(i).DaughterMomentum().at(0)/mc_shower->at(i).DaughterMomentum().at(3));       
      v1.SetY(mc_shower->at(i).DaughterMomentum().at(1)/mc_shower->at(i).DaughterMomentum().at(3));       
      v1.SetZ(mc_shower->at(i).DaughterMomentum().at(2)/mc_shower->at(i).DaughterMomentum().at(3));       
      mcs_Daughter_direction.at(i) = v1;

	}// loop over the mc_shower

	if(mcs_Daughter_energy.size()==2){
	// looking at how well we do just on getting the angle correct. 
	// here I will just use the corected values of E1 and E2
	    _mass_goodEnergy = Pi0MassFormula3D( mcs_Daughter_energy[0],
    			      mcs_Daughter_energy[1],
    			      ev_shower->at(0).Direction(),
    			      ev_shower->at(1).Direction());
	
        _mass_goodAngle = Pi0MassFormula3D(ev_shower->at(0).MIPEnergy().at(0),
    			      ev_shower->at(1).MIPEnergy().at(0),
    			      mcs_Daughter_direction[0],
    			      mcs_Daughter_direction[1]);
 
        _mass_detectorTrue = Pi0MassFormula3D( mcs_Daughter_energy[0],
    			      mcs_Daughter_energy[1],
    			      mcs_Daughter_direction[0],
    			      mcs_Daughter_direction[1]);



        _pi0_coscm = Pi0CosCM(ev_shower->at(0).MIPEnergy().at(0),ev_shower->at(1).MIPEnergy().at(0));

        _photon_dosReco = Pi0CosCM(mcs_Daughter_energy[0],mcs_Daughter_energy[1]);
        _photon_dosTrue = Pi0CosCM(ev_shower->at(0).MIPEnergy().at(0),ev_shower->at(1).MIPEnergy().at(0));
	// fill the hist with the good energy one	
    	hPi0MassPeak_GoodEnergy->Fill(_mass_goodEnergy);
    	//hPi0MassPeak_GoodAngle->Fill(_mass_goodAngle);
    	hPi0MassPeak_GoodAngle->Fill(_dorient_EE_calb*_mass_goodAngle);
    	hPi0MassPeak_GoodAnglePull->Fill(_dorient_EE_calb*_mass_goodAngle-_Pi0mass);
    	hPi0MassPeak_TrueDetector->Fill(_mass_detectorTrue);
    	hPi0CosCM->Fill(_pi0_coscm);
    	hPhotondos->Fill(_photon_dosReco,_photon_dosTrue);
	float recoangle = acos( mcs_Daughter_directio[0]* mcs_Daughter_directio[1] );
    	hopeningangle->Fill(recoangle,_photon_dosTrue);

	if(_photon_dosReco>0.4 && _photon_dosReco<0.8) hPi0MassPeakdoscut->Fill(3.6*_mass);
	}

 


     

    return true;
  }

  void ComputePi0Mass_RG::ComputeEnergyCorrection(storage_manager* storage)
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

  bool ComputePi0Mass_RG::finalize() {

    if(_fout) { 
      _fout->cd(); 
      hPi0MassPeak->Write();
      hPi0MassPeakPull->Write();
      hPi0MassPeak_GoodEnergy->Write();
      hPi0MassPeak_GoodAngle->Write();
      hPi0MassPeak_GoodAnglePull->Write();
      hPi0CosCM->Write();
      hPi0MassPeak_TrueDetector->Write();
      hPhotondos->Write();
      hPi0MassPeakdoscut->Write();




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
  float ComputePi0Mass_RG::Pi0MassFormula3D( float Energy1, float Energy2, TVector3 Direction3D_1, TVector3 Direction3D_2){
    
    float angle_3D = acos( Direction3D_1 * Direction3D_2 );

    //using a formula from 
    //http://www.hep.princeton.edu/~mcdonald/examples/piondecay.pdf      
    return pow(4. * Energy1 * Energy2 * pow(sin( 0.5 * angle_3D ),2) , 0.5);

  }

  //Get PI0 CosCM
  float ComputePi0Mass_RG::Pi0CosCM( float Energy1, float Energy2 ){
	
	float dos = fabs((Energy1 - Energy2))/(Energy1+Energy2);
	
	return dos;
	}// end of function for Pi0CosCM
 
}
#endif

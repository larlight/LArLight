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

    hPi0MassPeak_GoodEnergy = new TH1D("hPi0MassPeakGoodEnergy","Pi0 Mass Peak in MeV (True Photon Energy)",50,0,500);
    hPi0MassPeak_GoodAngle = new TH1D("hPi0MassPeakGoodAngle","Pi0 Mass Peak in MeV (True Opening Angle)",50,0,500);
    hPi0MassPeak_GoodAnglePull = new TH1D("hPi0MassPeakGoodAnglePull","Pi0 Mass Pull GoodAngle",50,-200,200);

    hPi0MassPeak_TrueDetector = new TH1D("hPi0MassPeakTrueDetector","Pi0 Mass Peak in MeV (True Detector: Photon Energy & Opening Angle) ",50,0,500);




    hPhotondos = new TH2D("hPhotondos","Photondos",50,0,1,50,0,1);
    hOpeningAngle = new TH2D("hPhotonOpening","PhotonAngle",32,0,3.14,32,0,3.14);

    hPi0MassPeakdoscut = new TH1D("hPi0MassPeakdoscut","Pi0 Mass Peak in MeV (Cut on (E1-E2)/(E1_E2) )",50,0,500);
    hPi0MassPeakanglecut = new TH1D("hPi0MassPeakanglecut","Pi0 Mass Peak in MeV (Cut Opening Angle )",50,0,500);
    hPi0MassPeakdosanglecut = new TH1D("hPi0MassPeakdosanglecut","Pi0 Mass Peak in MeV (Cut on Opening Angle and DOS)",50,0,500);


    hEnergyCorr_MomToDaughter = new TH1D("hEnergyCorr_MomToDaughter","Correction: hEnergyCorr_MomToDaughter",100,0.9,1.5);

    hElectronCorr_DepToDet = new TH1D("hElectronCorr_DepToDet","Correction: hElectronCorr_DepToDet",100,0,4);

	// make a tree 
    _ana_tree=0;
    MakeTTree();
	
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

    // Now get hit
    auto const hit_type = ev_cluster->get_hit_type();
    auto ev_hit    = (const event_hit*)(storage->get_data(hit_type));
    if(!ev_hit){
      print(MSG::ERROR,__FUNCTION__,Form("Associated hit not found!"));
      return false;
    }      


    //skip event if !2 showers found.
    if(ev_shower->size() != 2) return true;

    fEnergyCorr_MomToDaughter.resize(ev_shower->size(),1);
    fElectronCorr_DepToDet.resize(ev_shower->size(),1);
    fChargeCorr_DetToPeak.resize(ev_shower->size(),1);

    //if(_applyEnergyCorrection)
      //ComputeEnergyCorrection(storage);
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
      
      // walk-around ... use hit to get plane id since forgotten to store plane id in cluster! --kazu 
      auto const hit_index = ev_cluster->at(index).association(hit_type).at(0);
      auto const cluster_plane_id = ev_hit->at(hit_index).View();

      if(ev_shower->at(0).best_plane() == cluster_plane_id) break;

      ++best_cluster1;
    }

    size_t best_cluster2 = 0;
    for(auto const& index : ev_shower->at(1).association(_cluster_type)) {

      // walk-around ... use hit to get plane id since forgotten to store plane id in cluster! --kazu 
      auto const hit_index = ev_cluster->at(index).association(hit_type).at(0);
      auto const cluster_plane_id = ev_hit->at(hit_index).View();

      if(ev_shower->at(1).best_plane() == cluster_plane_id) break;

      ++best_cluster2;
    }
    std::cout<<best_cluster1<<" : "<<best_cluster2<<std::endl;
    _mass = Pi0MassFormula3D( ev_shower->at(0).MIPEnergy().at(best_cluster1), 
    			      ev_shower->at(1).MIPEnergy().at(best_cluster2),
    			      ev_shower->at(0).Direction(),
    			      ev_shower->at(1).Direction());

    
    std::cout<<"in compute thing, mass is "<<_mass<<std::endl;
    hPi0MassPeak->Fill(_dorient_EE_calb*_mass);
    hPi0MassPeakPull->Fill(_dorient_EE_calb*_mass -135);
    for(int i = 0; i<2; ++i){
      hEnergyCorr_MomToDaughter->Fill(fEnergyCorr_MomToDaughter.at(i));
      hElectronCorr_DepToDet->Fill(fElectronCorr_DepToDet.at(i));
    }

    std::vector<float> mcs_Daughter_energy(mc_shower->size(),0);
    std::vector<TVector3> mcs_Daughter_direction(mc_shower->size());
    std::vector<float> mcs_Mother_energy(mc_shower->size(),0);
    std::vector<TVector3> mcs_Mother_direction(mc_shower->size());
	
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
    	hPi0MassPeak_GoodAngle->Fill(_dorient_EE_calb*_mass_goodAngle);
    	hPi0MassPeak_GoodAnglePull->Fill(_dorient_EE_calb*_mass_goodAngle-_Pi0mass);
    	hPi0MassPeak_TrueDetector->Fill(_mass_detectorTrue);
    	hPi0CosCM->Fill(_pi0_coscm);
    	hPhotondos->Fill(_photon_dosReco,_photon_dosTrue);
	float truthangle = acos( mcs_Daughter_direction[0]* mcs_Daughter_direction[1] );
	float recoangle  = acos(ev_shower->at(0).Direction()*ev_shower->at(1).Direction());
    	hOpeningAngle->Fill(recoangle,truthangle);

	if(_photon_dosReco>0 && _photon_dosReco<0.8) hPi0MassPeakdoscut->Fill(_dorient_EE_calb*_mass);
	if(recoangle>0.4 && recoangle<1.0) hPi0MassPeakanglecut->Fill(_dorient_EE_calb*_mass);
	if(recoangle>0.4 && recoangle<1.0&&_photon_dosReco>0 && _photon_dosReco<0.8) hPi0MassPeakdosanglecut->Fill(_dorient_EE_calb*_mass);
	}

	// Filling of the tree variables 
	// Reco Variables
	
	E_1 = ev_shower->at(0).MIPEnergy().at(0);	
	E_2 = ev_shower->at(1).MIPEnergy().at(0);	
	E1E2DotProd  = acos(ev_shower->at(0).Direction()*ev_shower->at(1).Direction());
	

	// Truth Variables
	if( mc_shower->size() ==2 ){


     for(size_t i=0; i<mc_shower->size(); ++i) {
      mcs_Daughter_energy.at(i) = mc_shower->at(i).DaughterMomentum().at(3);
      mcs_Mother_energy.at(i) = mc_shower->at(i).MotherMomentum().at(3);
		//make a tvector to fit into the mass
      TVector3 v1;	
      TVector3 m1;	
      v1.SetX(mc_shower->at(i).DaughterMomentum().at(0)/mc_shower->at(i).DaughterMomentum().at(3));       
      v1.SetY(mc_shower->at(i).DaughterMomentum().at(1)/mc_shower->at(i).DaughterMomentum().at(3));       
      v1.SetZ(mc_shower->at(i).DaughterMomentum().at(2)/mc_shower->at(i).DaughterMomentum().at(3));       
      m1.SetX(mc_shower->at(i).MotherMomentum().at(0)/mc_shower->at(i).MotherMomentum().at(3));       
      m1.SetY(mc_shower->at(i).MotherMomentum().at(1)/mc_shower->at(i).MotherMomentum().at(3));       
      m1.SetZ(mc_shower->at(i).MotherMomentum().at(2)/mc_shower->at(i).MotherMomentum().at(3));       
      mcs_Daughter_direction.at(i) = v1;
      mcs_Mother_direction.at(i) = m1;

	}// loop over the mc_shower

	MC_E1E2DotProd  = acos(mcs_Daughter_direction.at(0)*mcs_Daughter_direction.at(1));
	MC_Mother_E1E2DotProd  = acos(mcs_Mother_direction.at(0)*mcs_Mother_direction.at(1));
        //Calculate theta and phi from directionality of momenta
        theta_d1 = asin( mcs_Daughter_direction.at(0).Y()  );
        phi_d1 = asin( mcs_Daughter_direction.at(0).X() / cos (theta_d1) ) ;

        theta_d2 = asin( mcs_Daughter_direction.at(1).Y()  );
        phi_d2 = asin( mcs_Daughter_direction.at(1).X() / cos (theta_d2) ) ;

	MC_E_1 = mc_shower->at(0).DaughterMomentum().at(3);
	MC_E_2 = mc_shower->at(1).DaughterMomentum().at(3);
	MC_Mother_E_1 = mc_shower->at(0).MotherMomentum().at(3);
	MC_Mother_E_2 = mc_shower->at(1).MotherMomentum().at(3);



	}// 
		
     if(_ana_tree) _ana_tree->Fill();
       
 


     

    return true;
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
      hOpeningAngle->Write();
      hPi0MassPeakdoscut->Write();
      hPi0MassPeakanglecut->Write();
      hPi0MassPeakdosanglecut->Write();


      hEnergyCorr_MomToDaughter->Write();
      hElectronCorr_DepToDet->Write();

	//Write the tree
    _ana_tree->Write();


      delete hPi0MassPeak;
      delete hEnergyCorr_MomToDaughter;
      delete hElectronCorr_DepToDet;
    }
    else 
      print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    
  
    return true;
  }

void ComputePi0Mass_RG::MakeTTree() {

    if(!_ana_tree) {
      _ana_tree = new TTree("ana_tree","");

          _ana_tree->Branch("E1",&E_1,"E_1/D");
          _ana_tree->Branch("E2",&E_2,"E_2/D");
          _ana_tree->Branch("DotProd",&E1E2DotProd,"E1E2DotProd/D");
          _ana_tree->Branch("theta_d1",&theta_d1,"theta_d1/D");
          _ana_tree->Branch("phi_d1",&phi_d1,"phi_d1/D");
          _ana_tree->Branch("theta_d2",&theta_d2,"theta_d2/D");
          _ana_tree->Branch("phi_d2",&phi_d2,"phi_d2/D");
          _ana_tree->Branch("MC_E_1",&MC_E_1,"MC_E_1/D");
          _ana_tree->Branch("MC_E_2",&MC_E_2,"MC_E_2/D");
          _ana_tree->Branch("MC_Mother_E_1",&MC_Mother_E_1,"MC_Mother_E_1/D");
          _ana_tree->Branch("MC_Mother_E_2",&MC_Mother_E_2,"MC_Mother_E_2/D");
          _ana_tree->Branch("MCDotProd",&MC_E1E2DotProd,"MC_E1E2DotProd/D");
          _ana_tree->Branch("MCMotherDotProd",&MC_Mother_E1E2DotProd,"MC_Mother_E1E2DotProd/D");
}

}// make the TTree


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


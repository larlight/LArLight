#ifndef RECOSHOWERQUALITY_CC
#define RECOSHOWERQUALITY_CC

#include "RecoShowerQuality.hh"

namespace larlight {

  bool RecoShowerQuality::initialize() {

    //NOTE: THIS CODE IS NOT GUARANTEED TO WORK ON EVENTS THAT HAVE MULTIPLE (TRUE AND/OR RECO) SHOWERS!
    //NEEDS FURTHER DEVELOPMENT TO HANDLE THOSE CASES

    //Variables to compare:
    //MC
    hMCShowerMulti = new TH1D("hMCShowerMulti","MC Shower Multiplicity; Number of MC Showers;",15,0,15);
    hMCShowerMotherMulti = new TH1D("hMCShowerMotherMulti","MC Shower Mother Multiplicity; Number of MC Shower Mothers;",15,0,15);
    hMCElectronMotherMulti = new TH1D("hMCElectronMotherMulti","MC Shower Mother=Electron Multiplicity; Number of MC Electron Mothers;",15,0,15);
    hMCElectronMom = new TH1D("hMCElectronMom","MC Shower Mother=Electron Momentum; Momentum;",100,0.,3.);
    hMCElectronEnergy = new TH1D("hMCElectronEnergy","MC Shower Mother=Electron Energy; Energy;",100,0.,3.);
    hMCOtherMom = new TH1D("hMCOtherMom","MC Shower Mother=Other Momentum; Momentum;",100,0.,0.1);//these should be all de-excitations from original electron showers
    hMCDepositedEnergy = new TH1D("hMCDepositedEnergy","MC Shower Mother=Electron Deposited Energy; Energy;",100,0.,5.);
    hMCShowerTheta = new TH1D("hMCShowerTheta","MC Shower Mother=Electron Theta; Theta (deg);",180,-90.,90.);
    hMCShowerPhi = new TH1D("hMCShowerPhi","MC Shower Mother=Electron Phi; Phi (deg);",360,-180.,180.);

    //Reco
    hRecoShowerMulti = new TH1D("hRecoShowerMulti","Reco Shower Multiplicity; Number of Reco Showers;",15,0,15);
    hRecoShowerCharge = new TH1D("hRecoShowerCharge","Reco Shower Charge; Charge;",100,0.,100.);
    hRecoShowerEn = new TH1D("hRecoShowerEn","Reco Shower Energy; Energy;",100,-1.,5.);
    hRecoShowerTheta = new TH1D("hRecoShowerTheta","Reco Shower Theta; Theta (deg);",180,-90.,90.);
    hRecoShowerPhi = new TH1D("hRecoShowerPhi","Reco Shower Phi; Phi (deg);",360,-180.,180.);

    //Diffs
    hShowerDirDiff = new TH1D("hShowerDirDiff","Shower Direction Difference; Angle (degrees);",360,-180.,180.);
    hShowerThetaAbsDiff = new TH1D("hShowerThetaAbsDiff","Shower Theta Diff (|Reco-Truth|); |Theta Diff| (deg);",180,0.,180.);
    hShowerPhiAbsDiff = new TH1D("hShowerPhiAbsDiff","Shower Phi Diff (|Reco-Truth|); |Phi Diff| (deg);",360,0.,360.);
    hShowerThetaDiff = new TH1D("hShowerThetaDiff","Shower Theta Diff (Reco-Truth); Theta Diff (deg);",360,-180.,180.);
    hShowerPhiDiff = new TH1D("hShowerPhiDiff","Shower Phi Diff (Reco-Truth); Phi Diff (deg);",720,-360.,360.);
    hShowerDCosXDiff = new TH1D("hShowerDCosXDiff","Shower DCosX Diff (Reco-Truth);;",100,-2.,2.);
    hShowerDCosYDiff = new TH1D("hShowerDCosYDiff","Shower DCosY Diff (Reco-Truth);;",100,-2.,2.);
    hShowerDCosZDiff = new TH1D("hShowerDCosZDiff","Shower DCosZ Diff (Reco-Truth);;",100,-2.,2.);
    hShowerEnergyRatioVsEnergy = new TH2D("hShowerEnergyRatioVsEnergy","Ratio (Reco Energy/True Energy) vs True Energy; True Energy; Ratio",
					  100,0.,3.,100,0.,100.);
    return true;
  }
  
  bool RecoShowerQuality::analyze(storage_manager* storage) {

    auto my_showers = (event_shower*)(storage->get_data(DATA::Shower));
    auto my_mcshowers = (event_mcshower*)(storage->get_data(DATA::MCShower));

    auto my_calo = (event_calorimetry*)(storage->get_data(DATA::Calorimetry));

    auto my_clusters = (event_cluster*)(storage->get_data(DATA::ShowerAngleCluster));
    DATA::DATA_TYPE my_hit_type = my_clusters->get_hit_type();
    auto my_hits = (event_hit*)(storage->get_data(my_hit_type));

    if (!my_showers){
      std::cerr << "I found no reco showers! skipping..." << std::endl;
      return false;
    }
    if (!my_mcshowers){
      std::cerr << "I found no mc showers! skipping..." << std::endl;
      return false;
    }
    if (!my_calo || !my_hits || !my_clusters){
      std::cerr << "I found no calorimetry, clusters or hits! skipping..." << std::endl;
      return false;
    }

    //Get shower multiplicities per event, as a cross check
    Int_t index = 0;
    for (auto this_shower : *my_showers){//loop over reconstructed 3d showers in event
      index++;
    }
    if (index >1){
      std::cerr << "This event has more than one reconstructed showers! Exiting..." << std::endl;
      return false;
    }
    hRecoShowerMulti->Fill(index);

    Int_t indexmc = 0;
    Int_t indexmcmother = 0;
    std::map<int, std::vector<int> > newmcshower;
    std::vector<int> mcshowerindex;
    for (auto this_mcshower : *my_mcshowers){//loop over true mc showers in event; there will be more than one, even for single e/gamma events
      indexmc++;
      //make a map of indexmc and mothertrackID; if this_mcshower.MotherTrackID already exists, append to vector
      if (newmcshower.find(this_mcshower.MotherTrackID()) == newmcshower.end()){//if this shower's mother id is not yet part of the map
      	mcshowerindex.clear();
     	mcshowerindex.push_back(indexmc);
      	newmcshower.insert(make_pair(this_mcshower.MotherTrackID(),mcshowerindex));
      }
      else {
      	mcshowerindex = (newmcshower.find(this_mcshower.MotherTrackID()))->second;
      	mcshowerindex.push_back(indexmc);
      	newmcshower.erase(this_mcshower.MotherTrackID());
      	newmcshower.insert(make_pair(this_mcshower.MotherTrackID(),mcshowerindex));
      }
    }
    indexmcmother = newmcshower.size();
    hMCShowerMulti->Fill(indexmc);
    hMCShowerMotherMulti->Fill(indexmcmother); //note: g4 will assign different mothertrackids even if the underlying source of the shower is the same (eg photon or electron). For example, a shower created from a gamma from some type of argon excitation which gives a new photon after some time/distance from original shower will likely be given a new trackid
    for (auto iter = newmcshower.begin(); iter != newmcshower.end(); ++iter){//this loops over map of MotherTrackID's
      if ((*iter).second.size() != 1){//each mother track id should be a unique shower!
	std::cerr << "This mother has more than one showers! Exiting..." << std::endl;
	return false;
      }
    }


    //Fill quality variables
    //for now, just consider showers whose mother track ID is an electron 
    //fill theta, phi, and directional cosines
    Double_t mcmomentum,mcenergy,mcdepenergy,mcphi,mctheta;
    mcmomentum = mcenergy = mcdepenergy = mcphi = mctheta = 0;
    TVector3 mcdcos = {0,0,0};
    int mcelectronindex = 0;
    for (auto this_mcshower : *my_mcshowers){//loop over mc showers
      mcmomentum = pow( pow(this_mcshower.MotherMomentum()[0],2) + pow(this_mcshower.MotherMomentum()[1],2) + pow(this_mcshower.MotherMomentum()[2],2) ,0.5);
      mcenergy = this_mcshower.MotherMomentum()[3];
      mcdepenergy = this_mcshower.DaughterMomentum()[3]/1000.;//in GeV
      //this is the theta used to construct directional cosines, also configured in prodsingle (YZ)
      //compare to:     /grid/fermiapp/larsoft/products/larsim/nightly/source/EventGenerator/SingleGen_module.cc 
      mctheta = asin(this_mcshower.MotherMomentum()[1]/mcmomentum) * 180./TMath::Pi();//py over p //asin define dfrom -90 to 90, so is theta
      //this is the phi used to construct directional cosines, also configured in prodsingle (XZ)
      mcphi = atan(this_mcshower.MotherMomentum()[0]/this_mcshower.MotherMomentum()[2]) * 180./TMath::Pi();//px over pz
      //note: atan is defined from -pi/2 to pi/2 whereas phi is defined from -180 to 180
      if (this_mcshower.MotherMomentum()[0]>0 && this_mcshower.MotherMomentum()[2]>0) {}//do nothing
      else if (this_mcshower.MotherMomentum()[0]>0 && this_mcshower.MotherMomentum()[2]<0) { mcphi -=180.; }
      else if (this_mcshower.MotherMomentum()[0]<0 && this_mcshower.MotherMomentum()[2]>0) {}//do nothing
      else if (this_mcshower.MotherMomentum()[0]<0 && this_mcshower.MotherMomentum()[2]<0) { mcphi +=180.; }

      if (this_mcshower.MotherPDGID() == 11){//electron ; there should be only one of these!
	hMCElectronMom->Fill(mcmomentum);
	hMCElectronEnergy->Fill(mcenergy);
	hMCDepositedEnergy->Fill(mcdepenergy);
	hMCShowerTheta->Fill(mctheta);
	hMCShowerPhi->Fill(mcphi);
	mcdcos[0] = sin(mcphi)*cos(mctheta);
	mcdcos[1] = sin(mctheta);
	mcdcos[2] = cos(mcphi)*cos(mctheta);	
	mcelectronindex++;
      }
      else {
	hMCOtherMom->Fill(mcmomentum);
      }
    }
    hMCElectronMotherMulti->Fill(mcelectronindex);

    TVector3 dcos = {0,0,0};
    Double_t energy,charge,phi,theta;
    energy = charge = phi = theta = 0;
    for (auto this_shower : *my_showers){
      dcos[0] = this_shower.Direction()[0];
      dcos[1] = this_shower.Direction()[1];
      dcos[2] = this_shower.Direction()[2];
      theta = asin(dcos[1]) * 180./TMath::Pi();//asin defined from -90 to 90, so is theta
      phi = atan(dcos[0]/dcos[2]) * 180./TMath::Pi();//atan defined over -pi/2 to pi/2, whereas phi is defined over -180 to 180
      if (dcos[0]>0 && dcos[2]>0) {}//do nothing
      if (dcos[0]>0 && dcos[2]<0) phi -= 180;
      if (dcos[0]<0 && dcos[2]>0) {}//do nothing
      if (dcos[0]<0 && dcos[2]<0) phi +=180;
      if (index!=0) hRecoShowerTheta->Fill(theta);//index!=0 ensures we're not looking at showers that didn't get reco'd
      if (index!=0) hRecoShowerPhi->Fill(phi);
    }

    //    if (index!=0 && indexmc==mcelectronindex){//indexmc==mcelectronindex is a quick fix to force comparison only with the electron mother
    if (index!=0){
      hShowerDCosXDiff->Fill(dcos[0]-mcdcos[0]);
      hShowerDCosYDiff->Fill(dcos[1]-mcdcos[1]);
      hShowerDCosZDiff->Fill(dcos[2]-mcdcos[2]);
      hShowerThetaAbsDiff->Fill(abs(theta-mctheta));
      hShowerPhiAbsDiff->Fill(abs(phi-mcphi));
      hShowerThetaDiff->Fill(theta-mctheta);
      hShowerPhiDiff->Fill(phi-mcphi);
    }

    Double_t angle = mcdcos.Angle(dcos) * 180./TMath::Pi();
    //    if (index!=0 && indexmc==mcelectronindex) hShowerDirDiff->Fill(angle);
    if (index!=0) hShowerDirDiff->Fill(angle);

    charge = 0;
    for (auto this_cluster : *my_clusters){
      for (auto hit_index : this_cluster.association(my_hit_type)){
	charge +=my_hits->at(hit_index).Charge();
      }
    }
    if (index!=0) hRecoShowerCharge->Fill(charge);

    for (auto this_calo : *my_calo){
      energy = this_calo.KineticEnergy()/1000.;//in GeV
      if (index!=0) hRecoShowerEn->Fill(energy);
    }

    //this is where the assumption of a single energy value per shower is heavily relied on
    //    if (index!=0 && indexmc==mcelectronindex) hShowerEnergyRatioVsEnergy->Fill(mcdepenergy,energy/mcdepenergy);
    if (index!=0) hShowerEnergyRatioVsEnergy->Fill(mcdepenergy,energy/mcdepenergy);  

    return true;
  }

  bool RecoShowerQuality::finalize() {

    if (_fout){//in ana there's an option for not outputting ana tree

      _fout->cd();

      //Store MC Truth histograms
      hMCShowerMulti->Write();
      hMCShowerMotherMulti->Write();
      hMCElectronMotherMulti->Write();
      hMCElectronMom->Write();
      hMCOtherMom->Write();
      hMCElectronEnergy->Write();
      hMCDepositedEnergy->Write();
      hMCShowerTheta->Write();
      hMCShowerPhi->Write();
      //Store Reco histograms
      hRecoShowerMulti->Write();
      hRecoShowerCharge->Delete();
      hRecoShowerEn->Write();
      hRecoShowerTheta->Write();
      hRecoShowerPhi->Write();
      //Store Comparison histograms
      hShowerDirDiff->Write();
      hShowerThetaAbsDiff->Write();
      hShowerPhiAbsDiff->Write();
      hShowerThetaDiff->Write();
      hShowerPhiDiff->Write();
      hShowerDCosXDiff->Write();
      hShowerDCosYDiff->Write();
      hShowerDCosZDiff->Write();
      hShowerEnergyRatioVsEnergy->Write();
    }


    // Delete objects on heap (made with "new") only if not written to TFile!
    /*
    //Delete MC Truth
    hMCShowerMulti->Delete();
    hMCShowerMotherMulti->Delete();
    hMCElectronMotherMulti->Delete();
    hMCElectronMom->Delete();
    hMCOtherMom->Delete();
    hMCElectronEnergy->Delete();
    hMCDepositedEnergy->Delete();
    hMCShowerTheta->Delete();
    hMCShowerPhi->Delete();
    //Delete Reco
    hRecoShowerMulti->Delete();
    hRecoShowerCharge->Delete();
    hRecoShowerEn->Delete();
    hRecoShowerTheta->Delete();
    hRecoShowerPhi->Delete();
    //Delete Comparison
    hShowerDirDiff->Delete();
    hShowerThetaAbsDiff->Delete();
    hShowerPhiAbsDiff->Delete();
    hShowerThetaDiff->Delete();
    hShowerPhiDiff->Delete();
    hShowerDCosXDiff->Delete();
    hShowerDCosYDiff->Delete();
    hShowerDCosZDiff->Delete();
    hShowerEnergyRatioVsEnergy->Delete();
    */
    return true;
  }
}
#endif

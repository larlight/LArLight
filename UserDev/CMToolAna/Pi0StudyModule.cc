#ifndef PI0STUDYMODULE_CC
#define PI0STUDYMODULE_CC

#include "Pi0StudyModule.hh"

namespace larlight {

  bool Pi0StudyModule::initialize() {

    ana_tree = 0;

    PrepareAnaTree();

    return true;
  }
  
  bool Pi0StudyModule::analyze(storage_manager* storage) {
    
    larlight::event_mctruth* ev_mctruth = (larlight::event_mctruth*)storage->get_data(larlight::DATA::MCTruth);
    if(!ev_mctruth) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, MCTruth!"));
      return false;
    }

    larlight::event_shower* ev_shower = (larlight::event_shower*)storage->get_data(larlight::DATA::Shower);
    if(!ev_shower) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, Shower!"));
      return false;
    }
    if(ev_shower->size() != 2){
      std::cout<<"Not 2 showers. Skip this event."<<std::endl;
      return false;
    }

        larlight::event_mcshower* ev_mcshower = (larlight::event_mcshower*)storage->get_data(larlight::DATA::MCShower);
    if(!ev_mcshower) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
      return false;
    }
    if(ev_mcshower->size() != 2){
      std::cout<<"Not 2 MCShowers. Skip this event."<<std::endl;
      return false;
    }


    //find the element in MC particle list that is the pi0
    //(can't assume it's 0)
    bool found = false;
    size_t pi0_particlelist_index;
    for(size_t i = 0; i < ev_mctruth->at(0).GetParticles().size(); ++i)
      if(ev_mctruth->at(0).GetParticles().at(i).PdgCode() == 111){
	pi0_particlelist_index = i;
	found = true;
	break;
      }
    
    if(!found){
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Problem! Didn't find pi0 PDG in the MC particle list!"));
      return false;
    }

    //get mother pi0 energy from mctruth [convert it GeV-->MeV]
    _MC_pi0_energy = ev_mctruth->at(0).GetParticles().at(pi0_particlelist_index).Trajectory().at(0).E() * 1000;

    //should probably do "best plane" stuff here. Already in MeV
    _reco_pi0_energy = ev_shower->at(0).MIPEnergy().at(2)+ev_shower->at(1).MIPEnergy().at(2);


    //this works only if ComputePi0Mass module is added to ana_processor
    //and runs before the Pi0StudyModule instance
    _mass = ComputePi0Mass::GetInstance()->GetMass();

    std::vector<double> pi0_mc_start_vtx(3,0);
    pi0_mc_start_vtx[0] = ev_mctruth->at(0).GetParticles().at(pi0_particlelist_index).Trajectory().at(0).X();
    pi0_mc_start_vtx[1] = ev_mctruth->at(0).GetParticles().at(pi0_particlelist_index).Trajectory().at(0).Y();
    pi0_mc_start_vtx[2] = ev_mctruth->at(0).GetParticles().at(pi0_particlelist_index).Trajectory().at(0).Z();
    
    //use TwoShower3DIntxn function
    //to find where these two showers would intersect in 3D space
    std::vector<double> start1(3,0);
    start1[0] = ev_shower->at(0).ShowerStart().X();
    start1[1] = ev_shower->at(0).ShowerStart().Y();
    start1[2] = ev_shower->at(0).ShowerStart().Z();

    std::vector<double> start2(3,0);
    start2[0] = ev_shower->at(1).ShowerStart().X();
    start2[1] = ev_shower->at(1).ShowerStart().Y();
    start2[2] = ev_shower->at(1).ShowerStart().Z();

    std::vector<double> reco_dir1(3,0);
    reco_dir1[0] = ev_shower->at(0).Direction().X();
    reco_dir1[1] = ev_shower->at(0).Direction().Y();
    reco_dir1[2] = ev_shower->at(0).Direction().Z();

    std::vector<double> reco_dir2(3,0);
    reco_dir2[0] = ev_shower->at(1).Direction().X();
    reco_dir2[1] = ev_shower->at(1).Direction().Y();
    reco_dir2[2] = ev_shower->at(1).Direction().Z();

    std::vector<double> pi0_reco_start_vtx_and_uncert = 
      TwoShower3DIntxn().FindIntxn(start1,reco_dir1,start2,reco_dir2);

    double mc_show1_magnitude = 
      pow(
	pow(ev_mcshower->at(0).DaughterMomentum()[0],2)+
	pow(ev_mcshower->at(0).DaughterMomentum()[1],2)+
	pow(ev_mcshower->at(0).DaughterMomentum()[2],2)
	,0.5);

    double mc_show2_magnitude = 
      pow(
	pow(ev_mcshower->at(1).DaughterMomentum()[0],2)+
	pow(ev_mcshower->at(1).DaughterMomentum()[1],2)+
	pow(ev_mcshower->at(1).DaughterMomentum()[2],2)
	,0.5);
    
    std::vector<double> mc_dir1(3,0); //made into unit vector
    mc_dir1[0] = ev_mcshower->at(0).DaughterMomentum()[0]/mc_show1_magnitude;
    mc_dir1[1] = ev_mcshower->at(0).DaughterMomentum()[1]/mc_show1_magnitude;
    mc_dir1[2] = ev_mcshower->at(0).DaughterMomentum()[2]/mc_show1_magnitude;
    
    std::vector<double> mc_dir2(3,0); //made into unit vector
    mc_dir2[0] = ev_mcshower->at(1).DaughterMomentum()[0]/mc_show2_magnitude;
    mc_dir2[1] = ev_mcshower->at(1).DaughterMomentum()[1]/mc_show2_magnitude;
    mc_dir2[2] = ev_mcshower->at(1).DaughterMomentum()[2]/mc_show2_magnitude;
    
    
    _dist_reco3Dlineintxn_MCorigin = 
      pow(pi0_mc_start_vtx.at(0)-pi0_reco_start_vtx_and_uncert.at(0),2)+
      pow(pi0_mc_start_vtx.at(1)-pi0_reco_start_vtx_and_uncert.at(1),2)+
      pow(pi0_mc_start_vtx.at(2)-pi0_reco_start_vtx_and_uncert.at(2),2);
    _dist_reco3Dlineintxn_MCorigin = pow(_dist_reco3Dlineintxn_MCorigin,0.5);
    
    _dist_reco3Dlineintxn_recoshow1start = 
      pow(ev_shower->at(0).ShowerStart().X()-pi0_reco_start_vtx_and_uncert.at(0),2)+
      pow(ev_shower->at(0).ShowerStart().Y()-pi0_reco_start_vtx_and_uncert.at(1),2)+
      pow(ev_shower->at(0).ShowerStart().Z()-pi0_reco_start_vtx_and_uncert.at(2),2);
    _dist_reco3Dlineintxn_recoshow1start = pow(_dist_reco3Dlineintxn_recoshow1start,0.5);

    _dist_reco3Dlineintxn_recoshow2start = 
      pow(ev_shower->at(1).ShowerStart().X()-pi0_reco_start_vtx_and_uncert.at(0),2)+
      pow(ev_shower->at(1).ShowerStart().Y()-pi0_reco_start_vtx_and_uncert.at(1),2)+
      pow(ev_shower->at(1).ShowerStart().Z()-pi0_reco_start_vtx_and_uncert.at(2),2);
    _dist_reco3Dlineintxn_recoshow2start = pow(_dist_reco3Dlineintxn_recoshow2start,0.5);

    _reco3Dlineintxn_uncert = pi0_reco_start_vtx_and_uncert.at(3);
   
    _dist_MC_momvtx_mcshow1start =
      pow(ev_mcshower->at(0).MotherPosition().at(0)-ev_mcshower->at(0).DaughterPosition().at(0),2)+
      pow(ev_mcshower->at(0).MotherPosition().at(1)-ev_mcshower->at(0).DaughterPosition().at(1),2)+
      pow(ev_mcshower->at(0).MotherPosition().at(2)-ev_mcshower->at(0).DaughterPosition().at(2),2);
    _dist_MC_momvtx_mcshow1start = pow(_dist_MC_momvtx_mcshow1start,0.5);

    _reco_opening_angle = 
      TMath::ACos(
	reco_dir1[0]*reco_dir2[0]+
	reco_dir1[1]*reco_dir2[1]+
	reco_dir1[2]*reco_dir2[2]
      );

    _mc_opening_angle = 
      TMath::ACos(
	mc_dir1[0]*mc_dir2[0]+
	mc_dir1[1]*mc_dir2[1]+
	mc_dir1[2]*mc_dir2[2]
      );

    _mc_min_opening_angle = 2*TMath::ASin( 134.97/_MC_pi0_energy );
    //    _reco_min_opening_angle = 2*TMath::ASin( _mass/_reco_pi0_energy );
    //note: this returns pi for reco_pi0_energy < 134.97
    _reco_min_opening_angle = 2*TMath::ASin( 134.97/_reco_pi0_energy );

    ana_tree->Fill();
    
    return true;
  }
  
  bool Pi0StudyModule::finalize() {

    if(_fout) { _fout->cd(); ana_tree->Write(); }

    if(ana_tree) delete ana_tree;
  
    return true;
  }


  void Pi0StudyModule::PrepareAnaTree(){

    if(!ana_tree) {
      
      ana_tree = new TTree("ana_tree","");

      ana_tree->Branch("mass",&_mass,"mass/F");
      ana_tree->Branch("mc_E",&_MC_pi0_energy,"mc_E/F");
      ana_tree->Branch("reco_E",&_reco_pi0_energy,"reco_E/F");
      ana_tree->Branch("dist_reco3Dlineintxn_MCorigin",&_dist_reco3Dlineintxn_MCorigin,"dist_reco3Dlineintxn_MCorigin/F");
      ana_tree->Branch("recoline3Dintxn_uncert",&_reco3Dlineintxn_uncert,"recoline3Dintxn_uncert/F");
      ana_tree->Branch("dist_reco3Dlineintxn_recoshow1start",&_dist_reco3Dlineintxn_recoshow1start,"dist_reco3Dlineintxn_recoshow1start/F");      
      ana_tree->Branch("dist_reco3Dlineintxn_recoshow2start",&_dist_reco3Dlineintxn_recoshow2start,"dist_reco3Dlineintxn_recoshow2start/F");      
      ana_tree->Branch("dist_MC_momvtx_mcshow1start",&_dist_MC_momvtx_mcshow1start,"dist_MC_momvtx_mcshow1start/F");

      ana_tree->Branch("mc_opening_angle",&_mc_opening_angle,"mc_opening_angle/F");
      ana_tree->Branch("reco_opening_angle",&_reco_opening_angle,"reco_opening_angle/F");
      ana_tree->Branch("mc_min_opening_angle",&_mc_min_opening_angle,"mc_min_opening_angle/F");
      ana_tree->Branch("reco_min_opening_angle",&_reco_min_opening_angle,"reco_min_opening_angle/F");

    }

  }
}
#endif

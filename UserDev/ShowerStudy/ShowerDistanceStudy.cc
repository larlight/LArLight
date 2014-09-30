#ifndef SHOWERDISTANCESTUDY_CC
#define SHOWERDISTANCESTUDY_CC

#include "ShowerDistanceStudy.hh"
#include "ShowerContainmentCut.hh"

namespace larlight {

  bool ShowerDistanceStudy::initialize() {

    _ana_tree=0;
    PrepareTTree();

    return true;
  }
  
  bool ShowerDistanceStudy::analyze(storage_manager* storage) {
 
	auto my_mcshower = (const event_mcshower*)(storage->get_data(DATA::MCShower)) ; 
 	
	_motherEnergy = my_mcshower->at(0).MotherMomentum().at(3) ;
	_daughter1_Energy = my_mcshower->at(0).DaughterMomentum().at(3) ;
	_daughter2_Energy = my_mcshower->at(1).DaughterMomentum().at(3) ;

	std::vector<double> dist1 ;
	std::vector<double> dist2 ;
	std::vector<double> momentum1;
	std::vector<double> momentum2;

	for(size_t i = 0; i<3 ; i++){
		dist1.push_back(my_mcshower->at(0).DaughterPosition().at(i)) ;
		dist2.push_back(my_mcshower->at(1).DaughterPosition().at(i)) ;
		momentum1.push_back(my_mcshower->at(0).DaughterMomentum().at(i)) ;
		momentum2.push_back(my_mcshower->at(1).DaughterMomentum().at(i)) ;
		}

	 showerana::ShowerContainmentCut showerObject ;

	_dist1_ToWall = showerObject.DistanceToWall(dist1) ;
	_dist1_AlongTraj = showerObject.DistanceToWall(dist1,momentum1);

	if(_ana_tree)
		_ana_tree->Fill();
 
    return true;
  }
void ShowerDistanceStudy::PrepareTTree() {

    if(!_ana_tree) {
      _ana_tree = new TTree("ana_tree","");


	  _ana_tree->Branch("_motherEnergy",&_motherEnergy,"_motherEnergy/D");
	  _ana_tree->Branch("_daughter1_Energy",&_daughter1_Energy,"_daughter1_Energy/D");
	  _ana_tree->Branch("_daughter2_Energy",&_daughter2_Energy,"_daughter2_Energy/D");

	  _ana_tree->Branch("_dist1_ToWall",&_dist1_ToWall,"_dist1_ToWall/D");
	  _ana_tree->Branch("_dist1_AlongTraj",&_dist1_AlongTraj,"_dist1_AlongTraj/D");

    }
 }



  bool ShowerDistanceStudy::finalize() {

    if(_fout) {

      _fout->cd();
      if(_ana_tree)
    	_ana_tree->Write();
      }
     else
       print(larlight::MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");

      delete _ana_tree;


    return true;
  }

}
#endif

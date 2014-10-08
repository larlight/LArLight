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

//	Clear(); 

	auto my_mcshower = (const event_mcshower*)(storage->get_data(DATA::MCShower)) ; 

	for( auto const & mcs : *my_mcshower) {

		/// Define vectors to fill for distance alg
		auto _kid_vtx 		= mcs.DaughterPosition(); 
		_kid_momentum  = mcs.DaughterMomentum() ;
		auto _mom_momentum  = mcs.MotherMomentum();

		_motherEnergy = _mom_momentum.at(3) ;		
		_daughterEnergy = _kid_momentum.at(3) ;


		showerana::ShowerContainmentCut showerObject ;
	
		_dist_ToWall = showerObject.DistanceToWall(_kid_vtx) ;
		_dist_AlongTraj = showerObject.DistanceToWall(_kid_vtx,_kid_momentum);
	if(_ana_tree)
		_ana_tree->Fill();
	}


 
    return true;
  }
void ShowerDistanceStudy::PrepareTTree() {

    if(!_ana_tree) {
      _ana_tree = new TTree("ana_tree","");


	  _ana_tree->Branch("_pi0_mass",&_pi0_mass,"_pi0_mass/D");
	  _ana_tree->Branch("_motherEnergy",&_motherEnergy,"_motherEnergy/D");
	  _ana_tree->Branch("_daughterEnergy",&_daughterEnergy,"daughterEnergy/D");

	  _ana_tree->Branch("_dist_ToWall",&_dist_ToWall,"dist_ToWall/D");
	  _ana_tree->Branch("_dist_AlongTraj",&_dist_AlongTraj,"dist_AlongTraj/D");

	  

    }
 }

/*void ShowerDistanceStudy::Clear() {

	_direction1.clear() ;
	_direction2.clear() ;

  }	
*/
 void Reset(){

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
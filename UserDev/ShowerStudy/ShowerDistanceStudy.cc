#ifndef SHOWERDISTANCESTUDY_CC
#define SHOWERDISTANCESTUDY_CC

#include "ShowerDistanceStudy.hh"
#include "DistToBoxWall.hh"

namespace larlight {

  bool ShowerDistanceStudy::initialize() {

    _ana_tree=0;
    PrepareTTree();

    return true;
  }
  
  bool ShowerDistanceStudy::analyze(storage_manager* storage) {

	Clear(); 

	auto my_mcshower = (const event_mcshower*)(storage->get_data(DATA::MCShower)) ; 

//	if(my_mcshower->size() > 0 ) 
//	std::cout<<"How many mc showers in each event? : "<<my_mcshower->size() <<std::endl;

	if(my_mcshower->size() > 0 ){
	for( auto const & mcs : *my_mcshower) {

		/// Define vectors to fill for distance alg
		auto _kid_vtx 		= mcs.DaughterPosition(); 
		auto _kid_momentum  = mcs.DaughterMomentum() ;
		auto _mom_momentum  = mcs.MotherMomentum();

		_motherEnergy = _mom_momentum.at(3) ; 
		_daughterEnergy = _kid_momentum.at(3) ;


		geoalgo::DistToBoxWall showerObject ;
	
		_dist_ToWall 		= showerObject.DistanceToWall(_kid_vtx) ;
		_dist_AlongTraj 	= showerObject.DistanceToWall(_kid_vtx,_kid_momentum,1);
		_dist_BackAlongTraj = showerObject.DistanceToWall(_kid_vtx,_kid_momentum,0);

		_dist_ToWallFromOut = showerObject.DistanceToWallFromOut( _kid_vtx ) ;

	if(_ana_tree)
		_ana_tree->Fill();
	}
}
 
    return true;
  }
void ShowerDistanceStudy::PrepareTTree() {

    if(!_ana_tree) {
      _ana_tree = new TTree("ana_tree","");


	  _ana_tree->Branch("_pi0_mass",&_pi0_mass,"_pi0_mass/D");
	  _ana_tree->Branch("_motherEnergy",&_motherEnergy,"_motherEnergy/D");
	  _ana_tree->Branch("_daughterEnergy",&_daughterEnergy,"_daughterEnergy/D");

	  _ana_tree->Branch("_dist_ToWall",&_dist_ToWall,"dist_ToWall/D");
	  _ana_tree->Branch("_dist_AlongTraj",&_dist_AlongTraj,"dist_AlongTraj/D");
	  _ana_tree->Branch("_dist_BackAlongTraj",&_dist_BackAlongTraj,"dist_BackAlongTraj/D");
	  _ana_tree->Branch("_dist_ToWallFromOut",&_dist_ToWallFromOut,"dist_ToWallFromOut/D");
	  

    }
 }

void ShowerDistanceStudy::Clear() {


  }	

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

#ifndef RECOSHOWERSTUDY_CC
#define RECOSHOWERSTUDY_CC

#include "RecoShowerStudy.hh"

namespace larlight {

  bool RecoShowerStudy::initialize() {

    _reco_tree = nullptr;
    PrepareTTree();
	_count0 = 0;
	_count1 = 0;

    return true;
  }
  
  bool RecoShowerStudy::analyze(storage_manager* storage) {
    
    ///Get info out of 3D reconstructed file
    auto my_recoshower = (const event_shower*)(storage->get_data(DATA::Shower)) ; 
//	std::vector<double> test(3,0) ;
//	std::vector<double> dir(3,0) ;	
    
    //	std::cout<<"Number of reco showers: "<<my_recoshower->size()<<std::endl;
    
    if(my_recoshower->size() ==2 || my_recoshower->size()==1 || my_recoshower->size()==3 ) _count0 ++ ; 
    
    
    for(auto const & mrs : *my_recoshower ){
      Clear();
      
      _count1++; 
      auto bestPlane = mrs.best_plane() ;
      _reco_energy.push_back(mrs.Energy().at(bestPlane)) ;

      for(size_t i = 0 ; i < my_recoshower->size(); i++)
      
      _direction.push_back(mrs.Direction().X());
      _direction.push_back(mrs.Direction().Y());
      _direction.push_back(mrs.Direction().Z());
      
      _shower_start.push_back(mrs.ShowerStart().X());
      _shower_start.push_back(mrs.ShowerStart().Y());
      _shower_start.push_back(mrs.ShowerStart().Z());
      
      showerana::ShowerContainmentCut showerObject ;
      
      _dist_to_wall 	  	  = showerObject.DistanceToWall(_shower_start) ;
      _dist_along_traj	      = showerObject.DistanceToWall(_shower_start,_direction,1);
	  _dist_back_along_traj   = showerObject.DistanceToWall(_shower_start,_direction,0);

/*	test[0] = 255;
	test[1] = -115; 
	test[2] = 1 ;

	dir[0] = -1; 
	dir[1] = 1 ;
	dir[2] = 1 ;

	_dist_to_wall = showerObject.DistanceToWall(test) ;
	_dist_along_traj = showerObject.DistanceToWall(test,dir) ;	  
	_dist_backwards_to_wall = showerObject.DistanceBackwardsToWall( test, dir) ; 

 	double add = _dist_along_traj + _dist_backwards_to_wall ; 
	
	  std::cout<<" "<<_shower_start[0]<<", "<<_shower_start[1]<<" , "<<_shower_start[2]<<std::endl;
	  std::cout<<" " <<_dist_along_traj<<", "<<_dist_backwards_to_wall <<std::endl;
	  std::cout<<" " <<  add <<std::endl;
  */    
      if(_reco_tree)
	_reco_tree->Fill();
      
    }
    
    return true;
  }
  
  void RecoShowerStudy::PrepareTTree() {

    if(!_reco_tree) {
      _reco_tree = new TTree("reco_tree","");

      _reco_tree->Branch("_shower_start","std::vector<double>",&_shower_start);
      _reco_tree->Branch("_direction","std::vector<double>",&_direction);
      _reco_tree->Branch("_reco_energy","std::vector<double>",&_reco_energy);

      _reco_tree->Branch("_dist_to_wall",&_dist_to_wall,"_dist_to_wall/D");
      _reco_tree->Branch("_dist_along_traj",&_dist_along_traj,"_dist_along_traj/D");
    
	  _reco_tree->Branch("_dist_back_along_traj",&_dist_back_along_traj,"_dist_back_along_traj/D");

    }   
 }

void RecoShowerStudy::Clear(){

	_direction.clear() ;	
	_shower_start.clear();
	_reco_energy.clear();


}


  bool RecoShowerStudy::finalize() {

    if(_fout) {

      _fout->cd();
      if(_reco_tree)
        _reco_tree->Write();
      }   
     else
       print(larlight::MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");

      delete _reco_tree;

	std::cout<<"\nNumber of 1,2,3 shower events is: "<<_count0<<std::endl;
	std::cout<<"All events: "<<_count1<<std::endl;
  
    return true;
  }

}
#endif

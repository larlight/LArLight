#ifndef MCTRUTH_CC
#define MCTRUTH_CC

#include "mctruth.hh"

namespace larlight {

  //######################################################
  void event_mc::clear_data()
  //######################################################
  {
    data_base::clear_data();
    clear();
    //fMCNeutrino.clear_data();
    fOrigin=MC::kUnknown;
    //fNeutrinoSet=false;
  }
  
  //######################################################
  part_mc::part_mc(int pdgid, int track, int parent, std::string process) 
    : data_base(),
      fProcess(process),
      fParentID(parent),
      fTrackID(track),
      fPDGID(pdgid)
  //######################################################
  {}
  
  //######################################################
  void part_mc::clear_data()
  //######################################################
  {
    data_base::clear_data();
    fStepTime.clear();
    fStepVertex.clear();
    fStepMomentum.clear();
    //fStepProcess.clear();
    fDaughters.clear();
    fProcess  = "DEFAULG";
    fParentID = -11;
    fTrackID  = -11;
    fPDGID    = -11;
  }
  
  //######################################################
  void part_mc::add_track(double x,  double y,  double z,  double t,
			  double px, double py, double pz)
  //######################################################
  {
    fStepTime.push_back(t);
    fStepVertex.push_back(TVector3(x,y,z));
    fStepMomentum.push_back(TVector3(px,py,pz));
  }
  
  //######################################################
  void event_mc::get_axis_range (double &max, double &min, const int axis) const
  //######################################################
  {
    
    if(axis < 0 || axis>2) {
      
      Message::get()->send(MSG::ERROR,__FUNCTION__,Form("Invalid axis index: %d",axis));
      
      return;
      
    }

    /*
      In DataFormat package, avoid using C++11 standard to keep this basic package compatible
      with old server machines.
    */
    for(size_t i=0; i<this->size(); ++i) {
      
      const std::vector<TVector3> vtx_v = this->at(i).step_vertex();
      
      for(size_t j=0; j<vtx_v.size(); ++j) {
	
	if((vtx_v.at(j))[axis] > max) max = (vtx_v.at(j))[axis];
	if((vtx_v.at(j))[axis] < min) min = (vtx_v.at(j))[axis];
	
      }
    }
  }
  
  //######################################################
  void event_mc::get_axis_range (double &xmax, double &xmin,
				 double &ymax, double &ymin,
				 double &zmax, double &zmin) const
  //######################################################
  {

    /*
      In DataFormat package, avoid using C++11 standard to keep this basic package compatible
      with old server machines.
    */    
    for(size_t i=0; i<this->size(); ++i) {
      
      const std::vector<TVector3> vtx_v = this->at(i).step_vertex();

      for(size_t j=0; j<vtx_v.size(); ++j) {
	
	if((vtx_v.at(j))[0] > xmax) xmax = (vtx_v.at(j))[0];
	if((vtx_v.at(j))[0] < xmin) xmin = (vtx_v.at(j))[0];
	if((vtx_v.at(j))[1] > ymax) ymax = (vtx_v.at(j))[1];
	if((vtx_v.at(j))[1] < ymin) ymin = (vtx_v.at(j))[1];
	if((vtx_v.at(j))[2] > zmax) zmax = (vtx_v.at(j))[2];
	if((vtx_v.at(j))[2] < zmin) zmin = (vtx_v.at(j))[2];
	
      }
    }
    
  }
}
#endif
  

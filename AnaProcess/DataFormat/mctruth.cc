#ifndef MCTRUTH_CC
#define MCTRUTH_CC

#include "mctruth.hh"

namespace larlight {
  //######################################################
  void event_mc::clear_data()
  //######################################################
  {
    data_base::clear_data();
    fPartList.clear();
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
    
    for(auto const& part : fPartList) {
      
      const std::vector<TVector3> vtx_v = part.step_vertex();
      
      for(auto const& vtx : vtx_v) {
	
	if(vtx[axis] > max) max = vtx[axis];
	if(vtx[axis] < min) min = vtx[axis];
	
      }
    }
  }
  
  //######################################################
  void event_mc::get_axis_range (double &xmax, double &xmin,
				 double &ymax, double &ymin,
				 double &zmax, double &zmin) const
  //######################################################
  {
    
    for(auto const& part : fPartList) {
      
      const std::vector<TVector3> vtx_v = part.step_vertex();
      
      for(auto const& vtx : vtx_v) {
	
	if(vtx[0] > xmax) xmax = vtx[0];
	if(vtx[0] < xmin) xmin = vtx[0];
	if(vtx[1] > ymax) ymax = vtx[1];
	if(vtx[1] < ymin) ymin = vtx[1];
	if(vtx[2] > zmax) zmax = vtx[2];
	if(vtx[2] < zmin) zmin = vtx[2];
	
      }
    }
    
  }
}
#endif
  

#ifndef MCTRUTH_CC
#define MCTRUTH_CC

#include "mctruth.hh"

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

#endif

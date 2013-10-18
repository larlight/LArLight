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
part_mc::part_mc(int pdgid, int track, int parent) : data_base()
//######################################################
{
  clear_data();
  fPDGID    = pdgid;
  fTrackID  = track;
  fParentID = parent;
}

//######################################################
void part_mc::clear_data()
//######################################################
{
  data_base::clear_data();
  fStepTime.clear();
  fStepVertex.clear();
  fStepMomentum.clear();
  fStepProcess.clear();
  fDaughters.clear();
  fParentID = -11;
  fTrackID  = -11;
  fPDGID    = -11;
}

#endif

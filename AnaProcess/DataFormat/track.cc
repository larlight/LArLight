#ifndef TRACK_CC
#define TRACK_CC

#include "track.hh"

//########################################
track::track() : data_base(),
		 fXYZ(),
		 fDir(),
		 fCov(),
		 fdQdx(),
		 fFitMomentum()
//########################################
{ clear_data(); }


//########################################
track::track(const track& original) :   fID(original.fID),
					fXYZ(original.fXYZ),
					fDir(original.fDir),
					fCov(original.fCov),
					fdQdx(original.fdQdx),
					fFitMomentum(original.fFitMomentum)
//########################################
{}

//########################################
void track::clear_data()
//########################################
{
  data_base::clear_data();  
  fID = -1; 
  fXYZ.clear(); 
  fDir.clear(); 
  fCov.clear(); 
  fdQdx.clear();
  fFitMomentum.clear(); 
}


#endif

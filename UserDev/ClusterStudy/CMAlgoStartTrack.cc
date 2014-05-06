#ifndef CMALGOSTARTTRACK_CC
#define CMALGOSTARTTRACK_CC

#include "CMAlgoStartTrack.hh"

namespace cluster {

  //-------------------------------------------------------
  CMAlgoStartTrack::CMAlgoStartTrack() : CBoolAlgoBase()
  //-------------------------------------------------------
  {

  }

  //-----------------------------
  void CMAlgoStartTrack::Reset()
  //-----------------------------
  {

  }

  //------------------------------------------------------------------------------------------
  void CMAlgoStartTrack::EventBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters)
  //------------------------------------------------------------------------------------------
  {

  }

  //-------------------------------
  void CMAlgoStartTrack::EventEnd()
  //-------------------------------
  {

  }

  //-----------------------------------------------------------------------------------------------
  void CMAlgoStartTrack::IterationBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters)
  //-----------------------------------------------------------------------------------------------
  {

  }

  //------------------------------------
  void CMAlgoStartTrack::IterationEnd()
  //------------------------------------
  {

  }
  
  //----------------------------------------------------------------
  bool CMAlgoStartTrack::Bool(const ClusterParamsAlgNew &cluster1,
			       const ClusterParamsAlgNew &cluster2)
  //----------------------------------------------------------------
  {
    return false;
  }

  //------------------------------
  void CMAlgoStartTrack::Report()
  //------------------------------
  {

  }
    
}
#endif

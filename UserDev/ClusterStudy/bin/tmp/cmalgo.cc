#ifndef CMALGO_CLASS_NAME_CXX
#define CMALGO_CLASS_NAME_CXX

#include "CMAlgo_Class_Name.hh"

namespace cluster {

  //-------------------------------------------------------
  CMAlgo_Class_Name::CMAlgo_Class_Name() : CBoolAlgoBase()
  //-------------------------------------------------------
  {

  }

  //-----------------------------
  void CMAlgo_Class_Name::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------
  bool CMAlgo_Class_Name::Bool(const ClusterParamsAlgNew &cluster1,
			       const ClusterParamsAlgNew &cluster2)
  //----------------------------------------------------------------
  {
    if(cluster1.GetNHits() && cluster2.GetNHits()) return false;
    else return false;
  }

  //------------------------------
  void CMAlgo_Class_Name::Report()
  //------------------------------
  {

  }
    
}
#endif

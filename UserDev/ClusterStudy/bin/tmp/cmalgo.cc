#ifndef CMALGO_CLASS_NAME_CC
#define CMALGO_CLASS_NAME_CC

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

  //------------------------------------------------------------------------------------------
  void CMAlgo_Class_Name::EventBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters)
  //------------------------------------------------------------------------------------------
  {

  }

  //-------------------------------
  void CMAlgo_Class_Name::EventEnd()
  //-------------------------------
  {

  }

  //-----------------------------------------------------------------------------------------------
  void CMAlgo_Class_Name::IterationBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters)
  //-----------------------------------------------------------------------------------------------
  {

  }

  //------------------------------------
  void CMAlgo_Class_Name::IterationEnd()
  //------------------------------------
  {

  }
  
  //----------------------------------------------------------------
  bool CMAlgo_Class_Name::Bool(const ClusterParamsAlgNew &cluster1,
			       const ClusterParamsAlgNew &cluster2)
  //----------------------------------------------------------------
  {
    return false;
  }

  //------------------------------
  void CMAlgo_Class_Name::Report()
  //------------------------------
  {

  }
    
}
#endif

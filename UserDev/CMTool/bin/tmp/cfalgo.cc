#ifndef CFALGO_CLASS_NAME_CXX
#define CFALGO_CLASS_NAME_CXX

#include "CFAlgo_Class_Name.hh"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgo_Class_Name::CFAlgo_Class_Name() : CBoolAlgoBase()
  //-------------------------------------------------------
  {

  }

  //-----------------------------
  void CFAlgo_Class_Name::Reset()
  //-----------------------------
  {

  }

  //---------------------------------------------------------------------------------
  bool CFAlgo_Class_Name::Float(std::vector<cluster::ClusterParamsAlgNew*> &clusters)
  //---------------------------------------------------------------------------------
  {
    if(cluster1.GetNHits() && cluster2.GetNHits()) return 1.;
    else return -1.;
  }

  //------------------------------
  void CFAlgo_Class_Name::Report()
  //------------------------------
  {

  }
    
}
#endif

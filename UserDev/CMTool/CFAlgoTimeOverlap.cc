#ifndef CFALGOTIMEOVERLAP_CXX
#define CFALGOTIMEOVERLAP_CXX

#include "CFAlgoTimeOverlap.hh"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoTimeOverlap::CFAlgoTimeOverlap() : CFloatAlgoBase()
  //-------------------------------------------------------
  {

  }

  //-----------------------------
  void CFAlgoTimeOverlap::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoTimeOverlap::Float(const std::vector<const cluster::ClusterParamsAlgNew*> &clusters)
  //----------------------------------------------------------------------------------------------
  {
 //	double start_cluster = fParams.start_point	;
//	double end_cluster = fParams.end_point 		;
	
   if(clusters.size()) return 1.;
    else return -1.;

/*	for(auto const& c1 : clusters){
		for(auto const& c2 : clusters){
		
		if(c1->GetParams()		
		



 }*/
  }

  //------------------------------
  void CFAlgoTimeOverlap::Report()
  //------------------------------
  {

  }
    
}
#endif

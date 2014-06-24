#ifndef CFALGOZOVERLAP_CXX
#define CFALGOZOVERLAP_CXX

#include "CFAlgoZOverlap.hh"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoZOverlap::CFAlgoZOverlap() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
		//_wire_ratio_cut = something ;
  }

  //-----------------------------
  void CFAlgoZOverlap::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoZOverlap::Float(const std::vector<const cluster::ClusterParamsAlgNew*> &clusters)
  //----------------------------------------------------------------------------------------------
  {
	if(clusters.size()) return 1.;
    else return -1.;

	double wire_distance 	= 0;
	double ratio 		= 1;
//	double length_max	= 0;	
	double max_wire_distance= -1;
	
	for(auto const& c : clusters){
	
		wire_distance = c->GetParams().end_point.w - c->GetParams().start_point.w ;

		if(max_wire_distance < wire_distance)
			max_wire_distance = wire_distance;
	
//		if(length_max < c->GetParams().length)
//    		length_max = c->GetParams().length;
		}

 	for(auto const& c : clusters){
	
			wire_distance = c->GetParams().end_point.w - c->GetParams().start_point.w ;

//			ratio *= c->GetParams().length / length_max ;
 			ratio *= wire_distance / max_wire_distance ; 
			
		}	
    
	

	//return(ratio > _wire_ratio_cut ? ratio : -1	)
	return(ratio) ;

  }

  //------------------------------
  void CFAlgoZOverlap::Report()
  //------------------------------
  {

  }
    
}
#endif

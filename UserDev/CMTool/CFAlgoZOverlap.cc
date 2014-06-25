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

	double wire_distance 		= 0;
	double ratio 				= 1;
	double max_wire_distance	= -1;

   /*	float start_wire1          = clusters.at(0)->GetParams().start_point.w ;
   	float end_wire1            = clusters.at(0)->GetParams().end_point.w ;
   	float start_wire2          = clusters.at(1)->GetParams().start_point.w ;
   	float end_wire2            = clusters.at(1)->GetParams().end_point.w ;
   	float start_wire3          = clusters.at(2)->GetParams().start_point.w ;
   	float end_wire3            = clusters.at(2)->GetParams().end_point.w ;
	
	float distance1 	   	   = end_wire1 - start_wire1 ;
	float distance2			   = end_wire2 - start_wire2 ;
	float distance3			   = end_wire3 - start_wire3 ; 
*/
	for(auto const& c : clusters){
	
		wire_distance = c->GetParams().end_point.w - c->GetParams().start_point.w ;

		if(max_wire_distance < wire_distance)
			max_wire_distance = wire_distance;
	
		}

 	for(auto const& c : clusters){
	
			wire_distance = c->GetParams().end_point.w - c->GetParams().start_point.w ;
 			ratio *= wire_distance / max_wire_distance ; 
			
		}	

	return(ratio) ;

  }

  //------------------------------
  void CFAlgoZOverlap::Report()
  //------------------------------
  {

  }
    
}
#endif

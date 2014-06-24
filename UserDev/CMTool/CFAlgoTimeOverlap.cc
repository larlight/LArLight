#ifndef CFALGOTIMEOVERLAP_CXX
#define CFALGOTIMEOVERLAP_CXX

#include "CFAlgoTimeOverlap.hh"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoTimeOverlap::CFAlgoTimeOverlap() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
		//_time_ratio_cut = something;
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
	
   if(clusters.size()) return 1.;
    else return -1.;

	double time_difference		 = 0;
	double max_time_difference	 = -1;
	double ratio				 = 1;

	for(auto const& c : clusters){

			time_difference = c->GetParams().end_point.t - c->GetParams().start_point.t;
//			std::cout<<"Testing..." <<c->GetParams().start_point.plane ;
		
			if(max_time_difference < time_difference)
				max_time_difference = time_difference ;

		}

	for(auto const& c : clusters){
			time_difference = c->GetParams().end_point.t - c->GetParams().start_point.t;
//			ratio *= time_difference/ max_time_difference ;

	    	//electron takes 1.875 us/plane
			if(time_difference >=0 && time_difference <= 3.75)
				ratio *= time_difference/ max_time_difference ;
			else ratio = 0 ;
		}


	if(_verbose)
		std::cout<<"Time overlap? "<<std::endl;
 	
return(ratio) ;

}


  //------------------------------
  void CFAlgoTimeOverlap::Report()
  //------------------------------
  {

  }
    
}
#endif

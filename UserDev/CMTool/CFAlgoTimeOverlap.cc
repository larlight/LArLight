#ifndef CFALGOTIMEOVERLAP_CXX
#define CFALGOTIMEOVERLAP_CXX

#include "CFAlgoTimeOverlap.hh"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoTimeOverlap::CFAlgoTimeOverlap() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
		_time_ratio_cut = 0.7 ;   //TEMPORARY

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
	
	double ratio				 = 1;
	double time_difference		 = 0;
	double max_time_difference	 = 0;


	for(auto const& c : clusters){
		time_difference  = c->GetParams().start_point.t - c->GetParams().end_point.t ; 

		if(time_difference < 0) 
			time_difference *= -1; 
	
		if(max_time_difference < time_difference)
			max_time_difference = time_difference ;
		}


	 for(auto const& c: clusters){
		time_difference  = c->GetParams().start_point.t - c->GetParams().end_point.t ; 
	
		if(time_difference<0)
			time_difference *= -1;
	
		ratio *= time_difference/max_time_difference ;

		if(ratio !=1 && ratio >0.5){	
		std::cout<<"\nStart point: "<<c->GetParams().start_point.t<<std::endl;
		std::cout<<"End Point: "<<c->GetParams().end_point.t  <<std::endl;
		std::cout<<"Max Time diff: "<<max_time_difference<<std::endl;
		std::cout<<"Time diff: "<<time_difference<<std::endl;
		std::cout<<"ratio for each cluster: "<<ratio<<std::endl;
		}
	}
		std::cout<<"Ratio is : "<<ratio<<std::endl; 	

 
	if(_verbose){
	//	std::cout<<"Time overlap? "<<std::endl;
//		std::cout<<"Max Time diff: "<<max_time_difference<<std::endl;
//		std::cout<<"Time diff: "<<time_difference<<std::endl;
		std::cout<<"Ratio is : "<<ratio<<std::endl; 	
	  }

	return (ratio > _time_ratio_cut ? ratio : -1 ); 

}


  //------------------------------
  void CFAlgoTimeOverlap::Report()
  //------------------------------
  {
  }
    
}
#endif

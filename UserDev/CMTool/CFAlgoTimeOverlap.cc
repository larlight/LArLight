#ifndef CFALGOTIMEOVERLAP_CXX
#define CFALGOTIMEOVERLAP_CXX

#include "CFAlgoTimeOverlap.hh"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoTimeOverlap::CFAlgoTimeOverlap() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
		_time_ratio_cut = 0.3 ;  //Cuts thus far unoptimized
		_N_Hits_cut		= 30 ; 
		_start_time_cut = 10 ;

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
	
    //Preserve location in time space. Cut clusters that have similar time differences,
	// but hit wires at very different times. 
	double start_t				 = 0;
	double end_t				 = 0;
	double prev_start_t 		 = 0;

	for(auto const& c : clusters){

		//Calculate time ratio for events with > 20 hits
		if(c->GetParams().N_Hits > _N_Hits_cut){
			time_difference  = c->GetParams().start_point.t - c->GetParams().end_point.t ; 
	
			if(time_difference < 0) 
				time_difference *= -1; 
	
			if(max_time_difference < time_difference)
				max_time_difference = time_difference ;
		 }
	}


	 for(auto const& c: clusters){
		if(c->GetParams().N_Hits > _N_Hits_cut){
			
     		//Make start_t always smaller
			if(c->GetParams().start_point.t > c->GetParams().end_point.t){
					start_t = c->GetParams().end_point.t   ;
					end_t   = c->GetParams().start_point.t ;
			 	}
			else{
					start_t = c->GetParams().start_point.t ;
					end_t	= c->GetParams().end_point.t   ;
				}	 
				
			if(prev_start_t ==0)
				prev_start_t = start_t ;

			time_difference  = end_t - start_t ; 
			ratio *= time_difference/max_time_difference ;
	
			//If current cluster's start time is not within some range of the previous cluster's start time,
			//modify ratio to disallow matching
			if( start_t < (prev_start_t - _start_time_cut) || start_t > (prev_start_t + _start_time_cut))
					ratio*= 0.01;

			prev_start_t = start_t ;

	   		
			/*if( ratio!= 1 && ratio > _time_ratio_cut ){	
				std::cout<<"\nStart point: "<<start_t<<std::endl;
				std::cout<<"End Point: "<<end_t  <<std::endl;
				std::cout<<"Previous start time: "<<prev_start_t<<std::endl;
				std::cout<<"ratio for each cluster: "<<ratio<<std::endl;
				} */

			}

		//If cluster has < N hits, modify ratio to disallow matching 
		else
			ratio *= 0.01 ;

		}

//		if(ratio > _time_ratio_cut) std::cout<<"MATCH FOUND WOOOOOOOOOOOOOOOOOOOO. Ratio is  : "<<ratio<<std::endl; 	
 
	if(_verbose){
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

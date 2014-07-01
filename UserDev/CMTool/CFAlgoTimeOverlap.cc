#ifndef CFALGOTIMEOVERLAP_CXX
#define CFALGOTIMEOVERLAP_CXX

#include "CFAlgoTimeOverlap.hh"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoTimeOverlap::CFAlgoTimeOverlap() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
		_time_ratio_cut 		= 0.1 ;  //Cuts thus far unoptimized
		_time_difference_cut	= 20 ; 
		_start_time_cut 		= 20 ;

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
			time_difference  = c->GetParams().start_point.t - c->GetParams().end_point.t ; 
	
			if(time_difference < 0) 
				time_difference *= -1; 
	
			if(max_time_difference < time_difference)
				max_time_difference = time_difference ;
	}


	 for(auto const& c: clusters){
		if(c->Plane()==0)
			ratio=1; 
	
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
	//		if( max_time_difference <= _time_difference_cut )
	//			ratio *= 1 ;
	//		else
			ratio *= time_difference/max_time_difference ;
	
			//If current cluster's start time is not within some range of the previous cluster's start time,
			//modify ratio to disallow matching
			if( start_t < (prev_start_t - _start_time_cut) || start_t > (prev_start_t + _start_time_cut))
					ratio*= 0.5;



			prev_start_t = start_t ;

	   		
			if( ratio > _time_ratio_cut ){	
				std::cout<<"\nPLANE: "<<c->Plane() ;
				std::cout<<"\nStart point: "<<start_t<<std::endl;
				std::cout<<"End Point: "<<end_t  <<std::endl;
				std::cout<<"Previous start time: "<<prev_start_t<<std::endl;
				std::cout<<"Max time diff: "<<max_time_difference<<std::endl;
				std::cout<<"ratio for each cluster: "<<ratio<<std::endl;
				
				} 

		}

	std::cout<<"*****************End of a cluster! "<<std::endl;
	if(ratio> _time_ratio_cut)
		std::cout<<"**************************FOUND A MATCH . ratio is: "<<ratio<<"\n\n\n"<<std::endl;
 
	return (ratio > _time_ratio_cut ? ratio : -1 ); 

}


  //------------------------------
  void CFAlgoTimeOverlap::Report()
  //------------------------------
  {
  }
    
}
#endif

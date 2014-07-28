#ifndef CFALGOTIMEOVERLAP_CXX
#define CFALGOTIMEOVERLAP_CXX

#include "CFAlgoTimeOverlap.hh"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoTimeOverlap::CFAlgoTimeOverlap() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
		SetRatioCut(0.001) ; //(0.095) ;  
		SetStartTimeCut(10) ;
		SetDebug(false) ;
		SetVerbose(false) ;

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
	double max_charge			 = 0;
	double charge_ratio 		 = 1;	
	//double adjusted_charge_ratio = 1;

    //Preserve location in time space. Cut clusters that have similar time differences,
	// but hit wires at very different times. 
	double start_t				 = 0;
	double end_t				 = 0;
	double prev_start_t 		 = 0;
	double prev_end_t 		 = 0;

	for(auto const& c : clusters){

		auto charge = c->GetParams().sum_charge ;		

		time_difference  = c->GetParams().start_point.t - c->GetParams().end_point.t ; 
	
		if(time_difference < 0) 
			time_difference *= -1; 
		
		if(max_time_difference < time_difference)
			max_time_difference = time_difference ;

		if(max_charge < charge)
			max_charge = charge ;
	
	}

	 for(auto const& c: clusters){
		if(c->Plane()==0){
			ratio=1;
			charge_ratio=1;
		 }
		
		double length = c->GetParams().length ;
		auto charge = c->GetParams().sum_charge ;		
	
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
			if(prev_end_t ==0)
				prev_end_t = end_t ;

			time_difference  = end_t - start_t ; 

			ratio *= time_difference/max_time_difference ;
//			charge_ratio *= charge/max_charge ;
//			adjusted_charge_ratio = charge_ratio * ratio ;
	
			ratio *= charge_ratio ;
	
			//If current cluster's start time is not within some range of the previous cluster's start time,
			//modify ratio to disallow matching
			
			if( (start_t > (prev_start_t - _start_time_cut) && start_t < (prev_start_t + _start_time_cut)) 
			   || (end_t > (prev_end_t - _start_time_cut) && end_t < (prev_end_t + _start_time_cut) )
			   || (length >25 && start_t >(prev_start_t - 2*_start_time_cut) && start_t < (prev_start_t + 2*_start_time_cut) ) )
				ratio *= 1;
			else	
				ratio*= 0.001;

			prev_start_t = start_t ;
			prev_end_t 	 = end_t ;	
		
			if( _verbose ){//&& ratio > _time_ratio_cut ){
				std::cout<<"\nPLANE: "<<c->Plane() ;
				std::cout<<"\nStart point: "<<start_t<<std::endl;
				std::cout<<"End Point: "<<end_t  <<std::endl;
			//	std::cout<<"Previous start time: "<<prev_start_t<<std::endl;
				std::cout<<"Time diff: "<<time_difference<<std::endl;
				std::cout<<"Max time diff: "<<max_time_difference<<std::endl;
				std::cout<<"Ratio for each cluster: "<<ratio<<std::endl;
//				std::cout<<"Charge: "<<charge<<std::endl;
//				std::cout<<"Charge Ratio: "<<charge_ratio<<std::endl;
//				std::cout<<"Adjusted Charge Ratio: "<<adjusted_charge_ratio<<std::endl;
				std::cout<<"Length and Width: "<<c->GetParams().length<<", "<<c->GetParams().width<<std::endl;
				} 

		}

	if(_verbose && ratio> _time_ratio_cut)
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

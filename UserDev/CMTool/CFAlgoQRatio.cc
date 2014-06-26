#ifndef CFALGOQRATIO_CXX
#define CFALGOQRATIO_CXX

#include "CFAlgoQRatio.hh"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoQRatio::CFAlgoQRatio() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
    _qratio_cut = 0.1 ;// Preliminary cuts
	_N_Hits_cut = 20 ;
	
  }

  //-----------------------------
  void CFAlgoQRatio::Reset()
  //-----------------------------
  {

  }

  //-----------------------------------------------------------------------------------------
  float CFAlgoQRatio::Float(const std::vector<const cluster::ClusterParamsAlgNew*> &clusters)
  //-----------------------------------------------------------------------------------------
  {
    
    double q_max = -1;
    float ratio = 1;

    // Search for max charge & return if any cluster has negative charge
	for(auto const& c : clusters) {

	  //Ignore all clusters with less than 40 hits 
	  if(c->GetParams().N_Hits >_N_Hits_cut){

    		if(c->GetParams().sum_charge < 0) {
				if(_verbose) 
					  std::cout 
					<< "\033[00m Found a cluster with negative charge!\033[00m ... aborting " 
					<< __FUNCTION__ <<std::endl;
				  	  return -1;
    	 		 }


	 		if(q_max < c->GetParams().sum_charge)
				q_max = c->GetParams().sum_charge;
        }

	 }

    // Compute ratio
    for(auto const& c : clusters){
		if(c->GetParams().N_Hits>_N_Hits_cut){
	        ratio *= c->GetParams().sum_charge / q_max;
	
			if(_verbose){
				std::cout<<"Sum Charge: "<<c->GetParams().sum_charge ;
				std::cout<<"Q max : "<<q_max<<std::endl ;
				std::cout<<"Ratio is: "<<ratio<<std::endl;	
			  }
		 }	
		//Modify the ratio of those cluster with < _N_Hits_cut to remove them from matching process
		else
			ratio*=0.1; 
	}
	
   		  if(_verbose) {
      	  	if( ratio > _qratio_cut ) std::cout << " ... pass!" << std::endl;
     	  	else std::cout << " ... below cut value: " << _qratio_cut << std::endl;
    		}

   	return ( ratio > _qratio_cut ? ratio : -1 );
 }
  

  //------------------------------
  void CFAlgoQRatio::Report()
  //------------------------------
  {

  }
    
}
#endif

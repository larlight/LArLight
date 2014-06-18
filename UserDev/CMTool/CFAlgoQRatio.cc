#ifndef CFALGOQRATIO_CXX
#define CFALGOQRATIO_CXX

#include "CFAlgoQRatio.hh"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoQRatio::CFAlgoQRatio() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
    _qratio_cut = 0.8;
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

    // Search for max charge & return if any cluster has negative charge
    for(auto const& c : clusters) {

      if(c->GetParams().sum_charge < 0) {

	if(_verbose) 
	  
	  std::cout 
	    << "\033[00m Found a cluster with negative charge!\033[00m ... aborting "
	    << __FUNCTION__
	    <<std::endl;

	return -1;

      }

      if(q_max < c->GetParams().sum_charge)

	q_max = c->GetParams().sum_charge;

    }

    // Compute ratio
    float ratio = 1;
    for(auto const& c : clusters)

      ratio *= c->GetParams().sum_charge / q_max;

    if(_verbose) {
      
      std::cout 
	<< "Computed ratio: " << ratio;

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

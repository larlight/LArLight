#ifndef CFALGOQRATIO_CXX
#define CFALGOQRATIO_CXX

#include "CFAlgoQRatio.hh"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoQRatio::CFAlgoQRatio() : CFloatAlgoBase()
  //-------------------------------------------------------
  {

  }

  //-----------------------------
  void CFAlgoQRatio::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoQRatio::Float(const std::vector<const cluster::ClusterParamsAlgNew*> &clusters)
  //----------------------------------------------------------------------------------------------
  {
    
    double q_max = -1;

    // Search for max charge & return if any cluster has negative charge
    for(auto const& c : clusters) {

      if(c->GetParams().sum_charge < 0) return -1;

      if(q_max < c->GetParams().sum_charge)

	q_max = c->GetParams().sum_charge;

    }

    // Compute ratio
    float ratio = 1;
    for(auto const& c : clusters)

      ratio *= c->GetParams().sum_charge / q_max;

    return ( ratio > _qratio_cut ? ratio : -1 );

  }

  //------------------------------
  void CFAlgoQRatio::Report()
  //------------------------------
  {

  }
    
}
#endif

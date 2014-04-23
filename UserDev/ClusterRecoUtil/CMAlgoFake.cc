#ifndef CMALGOFAKE_CC
#define CMALGOFAKE_CC

#include "CMAlgoFake.hh"

namespace cluster {

  CMAlgoFake::CMAlgoFake()
  {
    _flip = true;
    // Nothing to be done in the base class
  }
  
  bool CMAlgoFake::Merge(const cluster_params &cluster1,
			 const cluster_params &cluster2)
  {
    _flip = (!_flip);
    return _flip;
  }


}

#endif

#ifndef CMALGOFAKE_CXX
#define CMALGOFAKE_CXX

#include "CMAlgoFake.hh"

namespace cluster {

  //----------------------------------------
  CMAlgoFake::CMAlgoFake() : CBoolAlgoBase()
  //----------------------------------------
  {
    _flip = false;
    _ctr  = 0;
    // Nothing to be done in the base class
  }

  //--------------------------------------------------------
  bool CMAlgoFake::Bool(const ClusterParamsAlgNew &cluster1,
			const ClusterParamsAlgNew &cluster2)
  //--------------------------------------------------------
  {
    if(cluster1.GetNHits() && cluster2.GetNHits()) {
      _ctr++;
      if( (_ctr%64) == 0)
	_flip = (!_flip);
      return _flip;
    }
    else return false;
  }

}

#endif

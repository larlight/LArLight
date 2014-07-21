#ifndef CBALGOFAKE_CXX
#define CBALGOFAKE_CXX

#include "CBAlgoFake.hh"

namespace cmtool {

  //----------------------------------------
  CBAlgoFake::CBAlgoFake() : CBoolAlgoBase()
  //----------------------------------------
  {
    _flip = false;
    _ctr  = 0;
    // Nothing to be done in the base class
  }

  //--------------------------------------------------------
  bool CBAlgoFake::Bool(const ::cluster::ClusterParamsAlgNew &cluster1,
			const ::cluster::ClusterParamsAlgNew &cluster2)
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

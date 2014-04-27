#ifndef CMALGOFAKE_CC
#define CMALGOFAKE_CC

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
    _ctr++;
    if( (_ctr%64) == 0)
      _flip = (!_flip);
    return _flip;
  }

  //-----------------------
  void CMAlgoFake::Report()
  //-----------------------
  {
    std::cout<< "  I am just flpping every 64 counts... " << std::endl;
  }

}

#endif

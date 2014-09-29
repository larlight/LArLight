#ifndef SHOWERCONTAINMENTCUT_CC
#define SHOWERCONTAINMENTCUT_CC

#include "ShowerContainmentCut.hh"

namespace showerana {

  ShowerContainmentCut::ShowerContainmentCut()
    : _origin_vtx(3,0), _coord_max(3,0)
  {
    Reset();
  }

  void ShowerContainmentCut::Reset() 
  {
    _origin_vtx[0] = 0;
    _origin_vtx[1] = 0;
    _origin_vtx[2] = 0;

    _coord_max[0] = 256.;
    _coord_max[1] = 116.;
    _coord_max[2] = 1030.;

  }

  double ShowerContainmentCut::DistanceToWall(std::vector<double> const& point,
					      std::vector<double> const& dir) const
  {
    
    if( point.size() != 3 || dir.size() !=3) {

      std::ostringstream msg;
      msg 
	<< "<<" << __FUNCTION__ << ">>"
	<< " expects length 3 vector for point & dir ... given: "
	<< point.size() << " and " << dir.size()
	<< std::endl;
      throw ShowerAnaException(msg.str());
    }

    std::cout<<"\033[93m"<<"Not Yet Implemented!!!"<<"\033[00m"<<std::endl;
    return 0;
  }

}

#endif

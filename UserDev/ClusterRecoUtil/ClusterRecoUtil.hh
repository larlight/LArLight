#ifndef CLUSTERRECOUTIL_HH
#define CLUSTERRECOUTIL_HH 



#include "PxUtils.h"
#include <vector>

namespace cluster{
  class ClusterRecoUtil
  {
  public:
    ClusterRecoUtil();
    ~ClusterRecoUtil();

    double getNearestHitDistance(std::vector<larutil::PxHit> &, std::vector<larutil::PxHit> &);

    /* data */
  };

}


#endif
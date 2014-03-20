

#include "ClusterRecoUtil.hh"
#include <math.h>

double cluster::ClusterRecoUtil::getNearestHitDistance(std::vector<larutil::PxHit> & cluster1, std::vector<larutil::PxHit> & cluster2){
  if (cluster1.at(0).plane != cluster2.at(0).plane) return -999.;
  double minDistance = 1000.;
  double distance    = 1000.;

  for (auto & hit1 : cluster1){
    for(auto & hit2:cluster2){
      distance = (hit1.w - hit2.w)*(hit1.w - hit2.w) + (hit1.t - hit2.t)*(hit1.t - hit2.t);
      distance = sqrt(distance);
      if (distance <= minDistance) minDistance = distance;
    }
  }


  return minDistance;
}

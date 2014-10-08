#ifndef SHOWERRECOALGBASE_CXX
#define SHOWERRECOALGBASE_CXX

#include "ShowerRecoAlgBase.hh"

namespace showerreco {
  
  ShowerRecoAlgBase::ShowerRecoAlgBase() : fInputClusters()
  {
    fVerbosity = false;
  }

  void ShowerRecoAlgBase::Reset()
  {
    fInputClusters.clear();
  }

  void ShowerRecoAlgBase::AppendInputClusters(const std::vector< ::cluster::ClusterParamsAlg>& cpan_v)
  {
    std::vector< ::showerreco::ShowerCluster_t> clusters;
    clusters.reserve(cpan_v.size());

    for(auto const& cpan : cpan_v) {

      clusters.push_back( ::showerreco::ShowerCluster_t() );

      (*clusters.rbegin()).start_point = cpan.GetParams().start_point;
      (*clusters.rbegin()).end_point   = cpan.GetParams().end_point;
      (*clusters.rbegin()).angle_2d    = cpan.GetParams().angle_2d;
      (*clusters.rbegin()).plane_id    = cpan.Plane();
      (*clusters.rbegin()).hit_vector  = cpan.GetHitVector();

    }

    fInputClusters.push_back(clusters);

  }

  std::vector< ::larlight::shower> ShowerRecoAlgBase::Reconstruct()
  {

    ProcessInputClusters();

    std::vector< ::larlight::shower> output;

    output.reserve(fInputClusters.size());

    for(auto const& clusters : fInputClusters)

      output.push_back( RecoOneShower(clusters) );

    return output;

  }


}

#endif

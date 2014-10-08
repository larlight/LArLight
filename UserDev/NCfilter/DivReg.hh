/*
 * @author Ryan
 */


#ifndef DIVREG_HH
#define DIVREG_HH

#include <iostream>
#include <vector>
#include "PxUtils.h"
#include "ClusterParamsAlg.hh"
#include "LArUtilBase.hh"

namespace cluster {
  class DivReg{

  public:

    /// Default constructor
    DivReg(){};

    /// Default destructor
    virtual ~DivReg(){};

    /**
       Clustering function. Returns a vector of clusters.
       Each cluster is represented as a group of hit via vector of hit indexes.
    */
	// what ever I want it to return 
   // std::vector<std::vector<unsigned int> > Cluster(const std::vector<std::vector<larutil::PxHit>>& hits) const;
  std::vector<std::pair<double,double>> SplitLine(const std::vector<std::vector<larutil::PxHit>>& hits) const;
  std::vector<std::pair<double,double>> SplitLineC(const std::vector<larlight::cluster>& clusters,const std::vector<larlight::hit>& hits) const;
  std::vector<std::pair<double,double>> FlipLineC(const std::vector<larlight::cluster>& clusters,const std::vector<larlight::hit>& hits) const;
  std::vector<std::pair<double,double>> RotateLineC(const std::vector<larlight::cluster>& clusters,const std::vector<larlight::hit>& hits) const;

  };
}
#endif

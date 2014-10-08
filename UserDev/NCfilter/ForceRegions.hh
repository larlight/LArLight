/*
 * @author Ryan
 */


#ifndef FORCEREGIONS_HH
#define FORCEREGIONS_HH

#include <iostream>
#include <vector>
#include "PxUtils.h"
#include "ClusterParamsAlg.hh"
#include "LArUtilBase.hh"

namespace cluster {
  class ForceRegions{

  public:

    /// Default constructor
    ForceRegions(){};

    /// Default destructor
    virtual ~ForceRegions(){};

    /**
       Clustering function. Returns a vector of clusters.
       Each cluster is represented as a group of hit via vector of hit indexes.
    */
	// what ever I want it to return 
  std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  ForceTwoRegions(const std::vector<larlight::hit>& hits, std::vector<std::pair<double,double>> APP);

  std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  ForceTwoFlipRegions(const std::vector<larlight::hit>& hits, std::vector<std::pair<double,double>> APP);

  std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  ForceTwoRotateRegions(const std::vector<larlight::hit>& hits, std::vector<std::pair<double,double>> APP);

  };

}
#endif

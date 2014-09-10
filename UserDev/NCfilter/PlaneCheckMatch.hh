/*
 * @author Ryan
 */


#ifndef PLANECHECKMATCH_HH
#define PLANECHECKMATCH_HH

#include <iostream>
#include <vector>
#include "ana_base.hh"
#include "PxUtils.h"
#include "CRUHelper.hh"
#include "Quality.hh"
#include "ClusterParamsAlg.hh"
#include "LArUtilBase.hh"

namespace cluster {
  class PlaneCheckMatch{

  public:

    /// Default constructor
    PlaneCheckMatch(){};

    /// Default destructor
    virtual ~PlaneCheckMatch(){};

    /**
       Clustering function. Returns a vector of clusters.
       Each cluster is represented as a group of hit via vector of hit indexes.
    */
   
	// what ever I want it to return 
  std::vector<bool> PlaneCheck(const std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> Regions, const larlight::event_hit* hits  );



 protected:
        cluster::CRUHelper fCRUHelper;
        cluster::Quality fQuality;






  };
}
#endif

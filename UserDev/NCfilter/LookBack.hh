/*
 * @author Ryan
 */


#ifndef LOOKBACK_HH
#define LOOKBACK_HH

#include <iostream>
#include <vector>
#include "ana_base.hh"
#include "PxUtils.h"
#include "CRUHelper.hh"
#include "Quality.hh"
#include "ClusterParamsAlg.hh"
#include "LArUtilBase.hh"

namespace cluster {
  class LookBack{

  public:

    /// Default constructor
    LookBack(){};

    /// Default destructor
    virtual ~LookBack(){};

    /**
       Clustering function. Returns a vector of clusters.
       Each cluster is represented as a group of hit via vector of hit indexes.
    */
   
	// what ever I want it to return 
//  std::vector<bool> PlaneCheck(const std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> Regions, const larlight::event_hit* hits  );



 protected:
        //cluster::CRUHelper fCRUHelper;
        //cluster::Quality fQuality;






  };
}
#endif

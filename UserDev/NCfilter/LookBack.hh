/*
 * @author Ryan
 */


#ifndef LOOKBACK_HH
#define LOOKBACK_HH

#define PI 3.14159265

#include <iostream>
#include <vector>
#include "ana_base.hh"
#include "PxUtils.h"
#include "CRUHelper.hh"
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
  std::pair<double,double> CWAngle(std::vector<unsigned int> hitindex, std::vector<larlight::hit> hitsvect);

  std::pair<std::vector<unsigned int>,std::vector<unsigned int>> ClusterRefine(std::pair<std::vector<unsigned int>,std::vector<unsigned int>> hitpair, std::vector<larlight::hit> hitsvect);


 protected:
        //cluster::CRUHelper fCRUHelper;
        //cluster::Quality fQuality;

	double radius_of_interest=2; 





  };
}
#endif

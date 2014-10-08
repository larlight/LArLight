/*
 * @author Ryan
 */


#ifndef QUALITY_HH
#define QUALITY_HH

#include <iostream>
#include <vector>
#include "ana_base.hh"
#include "PxUtils.h"
#include "CRUHelper.hh"
#include "ClusterParamsAlg.hh"
#include "LArUtilBase.hh"

namespace cluster {
  class Quality{

  public:

    /// Default constructor
    Quality(){};

    /// Default destructor
    virtual ~Quality(){};

    /**
       Clustering function. Returns a vector of clusters.
       Each cluster is represented as a group of hit via vector of hit indexes.
    */
   
	// what ever I want it to return 
  double  TimeProf(std::vector<larutil::PxHit> hita ,std::vector<larutil::PxHit> hitb);
 // std::pair<double,double>  ProtoVertex(std::vector<larutil::PxHit> hita ,std::vector<larutil::PxHit> hitb);
	std::pair<double,double>  ProtoVertexcw(std::vector<larutil::PxHit> hita ,std::vector<larutil::PxHit> hitb);
	std::vector<std::pair<double,double>> ProtoVertexCW( const std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> Regions, const larlight::event_hit* hits ); 


 protected:
        cluster::CRUHelper fCRUHelper;






  };
}
#endif

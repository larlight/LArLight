/**
 * \file CMalgoPolyOverlap.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CBAlgoPolyOverlap
 *
 * @author David Caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CBALGOPOLYOVERLAP_HH
#define CBALGOPOLYOVERLAP_HH

#include <iostream>
#include "CBoolAlgoBase.hh"
#include "GeometryUtilities.hh"


namespace cmtool {
  /**
     \class CMalgoPolyContain
     Merge Polygons if the two overlap even partially
  */
  class CBAlgoPolyOverlap : public CBoolAlgoBase{
    
  public:
    
    /// Default constructor
    CBAlgoPolyOverlap();
    
    /// Default destructor
    virtual ~CBAlgoPolyOverlap(){};
 
    /**
       Core function: given the ClusterParamsAlgNew input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::ClusterParamsAlgNew &cluster1,
		      const ::cluster::ClusterParamsAlgNew &cluster2);

    void SetDebug(bool debug) { _debug = debug; }

    //both clusters must have > this # of hits to be considered for merging
    void SetMinNumHits(size_t nhits) { _min_hits = nhits; }

    /// Method to re-configure the instance
    void reconfigure();

  private:
    
    bool _debug;
    size_t _min_hits;
  };
}

#endif
/** @} */ // end of doxygen group 


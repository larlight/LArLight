/**
 * \file CMalgoPolyOverlap.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CMAlgoPolyOverlap
 *
 * @author David Caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CMALGOPOLYOVERLAP_HH
#define CMALGOPOLYOVERLAP_HH

#include <iostream>
#include "CBoolAlgoBase.hh"
#include "GeometryUtilities.hh"


namespace cluster {
  /**
     \class CMalgoPolyContain
     Merge Polygons if the two overlap even partially
  */
  class CMAlgoPolyOverlap : public CBoolAlgoBase{
    
  public:
    
    /// Default constructor
    CMAlgoPolyOverlap();
    
    /// Default destructor
    virtual ~CMAlgoPolyOverlap(){};
 
    /**
       Core function: given the ClusterParamsAlgNew input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ClusterParamsAlgNew &cluster1,
		      const ClusterParamsAlgNew &cluster2);

    void SetDebug(bool debug) { _debug = debug; }

    void SetVerbose(bool verbose) { _verbose = verbose; }

    /// Method to re-configure the instance
    void reconfigure();

  private:
    
    bool _verbose;
    bool _debug;

  };
}

#endif
/** @} */ // end of doxygen group 

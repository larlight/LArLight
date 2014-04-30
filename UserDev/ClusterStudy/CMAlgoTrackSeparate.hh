/**
 * \file CMAlgoFake.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CMAlgoTrackSeparate
 *
 * @author david caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CMALGOTRACKSEPARATE_HH
#define CMALGOTRACKSEPARATE_HH

#include <iostream>
#include "CBoolAlgoBase.hh"

namespace cluster {
  /**
     \class CMAlgoTrackSeparate
  */
  class CMAlgoTrackSeparate : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CMAlgoTrackSeparate();
    
    /// Default destructor
    virtual ~CMAlgoTrackSeparate(){};

    /**
       Core function: given the ClusterParamsAlgNew input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ClusterParamsAlgNew &cluster1,
		      const ClusterParamsAlgNew &cluster2);

    void SetVerbose(bool on) { _verbose = on; }

    void SetMinNumHits(int n) { _MinNumHits = n; }

    void SetMinAngleDiff(float anglesep) { _MinAngleDiff = anglesep; }
    
    void SetMaxOpeningAngle(float maxangle) { _MaxOpeningAngle = maxangle; }

    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

    /// Function to report what's going on per merging
    virtual void Report();

  protected:

    bool _verbose;
    int _MinNumHits;
    float _MinAngleDiff;
    float _MaxOpeningAngle;
    double _wire_2_cm;
    double _time_2_cm;
  };
}

#endif
/** @} */ // end of doxygen group 


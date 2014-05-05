/**
 * \file CMAlgoStartNearEnd.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CMAlgoStartNearEnd
 *
 * @author david caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CMALGOSTARTNEAREND_HH
#define CMALGOSTARTNEAREND_HH

#include <iostream>
#include "CBoolAlgoBase.hh"

namespace cluster {
  /**
     \class CMAlgoStartNearEnd
     Merge if start point of one is near end point of another
     and require angle compatibility
  */
  class CMAlgoStartNearEnd : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CMAlgoStartNearEnd();
    
    /// Default destructor
    virtual ~CMAlgoStartNearEnd(){};
 
    /**
       Core function: given the ClusterParamsAlgNew input, return whether a cluster should be
       merged or not.
    */

    /// Function to set verbosity
    void SetVerbose(bool verbosity) { _verbose=verbosity; }

    void SetMaxStartEndSeparation(double d) { _separation=d; }

    void SetMaxAngle(double a) { _maxopeningangle=a; }

    void SetMinHits(int n) { _MinHits=n; }

    virtual bool Bool(const ClusterParamsAlgNew &cluster1,
		      const ClusterParamsAlgNew &cluster2);

    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

    /// Function to report what's going on per merging
    virtual void Report();

  protected:

    bool _verbose;
    double _maxopeningangle;
    double _separation;
    int _MinHits;

  };
}

#endif
/** @} */ // end of doxygen group 


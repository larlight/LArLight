/**
 * \file CFAlgoWireOverlap.hh
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CFAlgoWireOverlap
 *
 * @author david_NAME
 */

/** \addtogroup CMTool

    @{*/
#ifndef CFALGOWIREOVERLAP_HH
#define CFALGOWIREOVERLAP_HH

#include "CFloatAlgoBase.hh"
#include <math.h>
#include "Polygon2D.hh"

namespace cmtool {
  /**
     \class CFAlgoWireOverlap
     User implementation for CFloatAlgoBase class
     doxygen documentation!
  */
  class CFAlgoWireOverlap : public CFloatAlgoBase {
    
  public:
    
    /// Default constructor
    CFAlgoWireOverlap();
    
    /// Default destructor
    virtual ~CFAlgoWireOverlap(){};

    //
    // Author should be aware of 3 functions at least: Float, Report, 
    // and Reset. More possibly-useful functions can be found in the later 
    // part but commented out. All of these functions are virtual and defined
    // in the base class.

    /**
       Core function: given a set of CPANs, return a float which indicates 
       the compatibility the cluster combination.
    */
    virtual float Float(const std::vector<const cluster::ClusterParamsAlg*> &clusters);

    /**
       Optional function: called after each iterative approach if a manager class is
       run with verbosity level <= kPerIteration. Maybe useful for debugging.
    */
    virtual void Report();
    
    /// Function to reset the algorithm instance, called together with manager's Reset()
    virtual void Reset();

    /// Function to set verbose output
    void SetVerbose( bool on ) { _verbose = on; }

  private:

    double _w2cm, _t2cm;
    bool   _verbose;
  };


}
#endif
/** @} */ // end of doxygen group 


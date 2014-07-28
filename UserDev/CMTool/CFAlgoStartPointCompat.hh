/**
 * \file CFAlgoStartPointCompat.hh
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CFAlgoStartPointCompat
 *
 * @author david_NAME
 */

/** \addtogroup CMTool

    @{*/
#ifndef CFALGOSTARTPOINTCOMPAT_HH
#define CFALGOSTARTPOINTCOMPAT_HH

#include "CFloatAlgoBase.hh"
#include <math.h>

namespace cmtool {
  /**
     \class CFAlgoStartPointCompat
     User implementation for CFloatAlgoBase class
     doxygen documentation!
  */
  class CFAlgoStartPointCompat : public CFloatAlgoBase {
    
  public:
    
    /// Default constructor
    CFAlgoStartPointCompat();
    
    /// Default destructor
    virtual ~CFAlgoStartPointCompat(){};

    //
    // Author should be aware of 3 functions at least: Float, Report, 
    // and Reset. More possibly-useful functions can be found in the later 
    // part but commented out. All of these functions are virtual and defined
    // in the base class.

    /**
       Core function: given a set of CPANs, return a float which indicates 
       the compatibility the cluster combination.
    */
    virtual float Float(const std::vector<const cluster::ClusterParamsAlgNew*> &clusters);

    /**
       Optional function: called after each iterative approach if a manager class is
       run with verbosity level <= kPerIteration. Maybe useful for debugging.
    */
    virtual void Report();
    
    /// Function to reset the algorithm instance, called together with manager's Reset()
    virtual void Reset();

    void UseTime( bool on ) { _time = on; }
    
  private:

    double _w2cm, _t2cm;
    double _MaxArea;
    bool _time;  /// Use Time to compute intersection and trinagle area

  };


}
#endif
/** @} */ // end of doxygen group 


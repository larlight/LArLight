/**
 * \file CMAlgo_Class_Name.hh
 *
 * \ingroup Working_Package
 * 
 * \brief Class def header for a class CMAlgo_Class_Name
 *
 * @author USER_NAME
 */

/** \addtogroup Working_Package

    @{*/
#ifndef CMALGO_CLASS_NAME_HH
#define CMALGO_CLASS_NAME_HH

#include <iostream>
#include "CMergeAlgoBase.hh"

namespace cluster {
  /**
     \class CMAlgo_Class_Name
     User implementation for CMergeAlgoBase class
     doxygen documentation!
  */
  class CMAlgo_Class_Name : public CMergeAlgoBase {
    
  public:
    
    /// Default constructor
    CMAlgo_Class_Name();
    
    /// Default destructor
    virtual ~CMAlgo_Class_Name(){};

    /**
       Core function: given the CPAN input, return whether a cluster should be
       merged or not.
    */
    virtual bool Merge(const ClusterParamsAlgNew &cluster1,
		       const ClusterParamsAlgNew &cluster2);

    /**
       Optional function: Called before iterating over 
     */
    virtual void Prepare(const std::vector<cluster::ClusterParamsAlgNew> &clusters);
    
    /**
       Optional function: called after each Merge() function call by CMergeManager IFF
       CMergeManager is run with verbosity level kPerMerging. Maybe useful for debugging.
    */
    virtual void Report();
    
    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset();
    
  };
}
#endif
/** @} */ // end of doxygen group 


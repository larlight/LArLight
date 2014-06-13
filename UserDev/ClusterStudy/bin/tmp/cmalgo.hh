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
#include "CBoolAlgoBase.hh"

namespace cluster {
  /**
     \class CMAlgo_Class_Name
     User implementation for CBoolAlgoBase class
     doxygen documentation!
  */
  class CMAlgo_Class_Name : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CMAlgo_Class_Name();
    
    /// Default destructor
    virtual ~CMAlgo_Class_Name(){};

    //
    // Author should be aware of 3 functions at least: Bool, Report, and Reset.
    // More possibly-useful functions can be later part but commented out.
    // All of these functions are virtual and defined in the base class.
    //

    /**
       Core function: given the CPAN input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ClusterParamsAlgNew &cluster1,
		      const ClusterParamsAlgNew &cluster2);

    /**
       Optional function: called after each Merge() function call by CMergeManager IFF
       CMergeManager is run with verbosity level kPerMerging. Maybe useful for debugging.
    */
    virtual void Report();
    
    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset();
    


    /**
       Optional function: called at the beginning of 1st iteration. This is called per event.
     */
    //virtual void EventBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters);

    /**
       Optional function: called at the end of event ... after the last merging iteration is over.
     */
    //virtual void EventEnd();
 
    /**
       Optional function: called at the beggining of each iteration over all pairs of clusters. 
       This provides all clusters' information in case the algorithm need them. Note this
       is called per iteration which may be more than once per event.
     */
    //virtual void IterationBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters);

    /**
       Optional function: called at the end of each iteration over all pairs of clusters.
     */
    //virtual void IterationEnd();

  };
}
#endif
/** @} */ // end of doxygen group 


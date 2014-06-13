/**
 * \file CMAlgoAngleIncompat.hh
 *
 * \ingroup ClusterStudy
 * 
 * \brief Class def header for a class CMAlgoAngleIncompat
 *
 * @author davidkaleko_NAME
 */

/** \addtogroup ClusterStudy

    @{*/
#ifndef CMALGOANGLEINCOMPAT_HH
#define CMALGOANGLEINCOMPAT_HH

#include <iostream>
#include "CBoolAlgoBase.hh"

namespace cluster {
  /**
     \class CMAlgoAngleIncompat
     User implementation for CBoolAlgoBase class
     doxygen documentation!
  */
  class CMAlgoAngleIncompat : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CMAlgoAngleIncompat();
    
    /// Default destructor
    virtual ~CMAlgoAngleIncompat(){};

    /**
       Optional function: called at the beginning of 1st iteration. This is called per event.
     */
    virtual void EventBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters);

    /**
       Optional function: called at the end of event ... after the last merging iteration is over.
     */
    virtual void EventEnd();
 
    /**
       Optional function: called at the beggining of each iteration over all pairs of clusters. 
       This provides all clusters' information in case the algorithm need them. Note this
       is called per iteration which may be more than once per event.
     */
    virtual void IterationBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters);

    /**
       Optional function: called at the end of each iteration over all pairs of clusters.
     */
    virtual void IterationEnd();

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


    void SetAllow180Ambig(bool on) { _allow_180_ambig = on; }

    void SetAngleCut(double angle) { _max_allowed_2D_angle_diff = angle; }

    void SetUseOpeningAngle(bool on) { _use_opening_angle = on; }

    void SetMinHits(int n) { _minHits = n; }

    void SetMinLength(double l) { _min_length = l; }

    void SetDebug(bool on) { _debug = on; }

  protected:
   
    bool _debug;
    bool _allow_180_ambig;
    double _max_allowed_2D_angle_diff;
    double _min_length;
    bool _use_opening_angle;
    size_t _minHits;
  };
}
#endif
/** @} */ // end of doxygen group 


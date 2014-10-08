/**
 * \file CFAlgoTimeOverlap.hh
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CFAlgoTimeOverlap
 *
 * @author ariana hackenburg
 */

/** \addtogroup CMTool

    @{*/
#ifndef CFALGOTIMEOVERLAP_HH
#define CFALGOTIMEOVERLAP_HH

#include "CFloatAlgoBase.hh"

namespace cmtool {
  /**
     \class CFAlgoTimeOverlap
     User implementation for CFloatAlgoBase class
     doxygen documentation!
  */
  class CFAlgoTimeOverlap : public CFloatAlgoBase {
    
  public:
    
    /// Default constructor
    CFAlgoTimeOverlap();
    
    /// Default destructor
    virtual ~CFAlgoTimeOverlap(){};

    /**This algorithm calculates the difference between start and end times for merged clusters,
		and compares across planes to form matches. 
    */
    virtual float Float(const std::vector<const cluster::ClusterParamsAlg*> &clusters);

	void SetStartTimeCut(float start_time) { _start_time_cut = start_time ; } 

	void SetRatioCut(float ratio) { _time_ratio_cut = ratio ; } 

 
    //Order the theta, phi, hits per plane to make cuts convenient
    virtual void SetMaxMiddleMin(const double first, const double second, const double third, double &most, double &middle, double &least) ;

	void SetDebug(bool debug) { _debug = debug ; }

	void SetVerbose(bool verbose) { _verbose = verbose ; }

    virtual void Report();
    
    virtual void Reset();

	protected:
		float _time_ratio_cut ;
		float _start_time_cut ;
		bool _debug ;
		bool _verbose ;

  };
}
#endif
/** @} */ // end of doxygen group 


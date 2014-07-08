/**
 * \file CFAlgo3DAngle.hh
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CFAlgo3DAngle
 *
 * @author ah673_NAME
 */

/** \addtogroup CMTool

    @{*/
#ifndef CFALGO3DANGLE_HH
#define CFALGO3DANGLE_HH

#include "CFloatAlgoBase.hh"

namespace cmtool {
  /**
     \class CFAlgo3DAngle
     User implementation for CFloatAlgoBase class
     doxygen documentation!
  */
  class CFAlgo3DAngle : public CFloatAlgoBase {
    
  public:
    
    /// Default constructor
    CFAlgo3DAngle();
    
    /// Default destructor
    virtual ~CFAlgo3DAngle(){};

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
		If phi or theta are <360 (or greater than), adjust them to be in a more easily
		comparable range.
	*/
	virtual void FixPhiTheta(double &phi, double &theta) ;

	/**
		Set max and min phi and theta for ratio calculations
	*/
	virtual void SetMaxMin(const double phi_1, const double phi_2,double &max_1,double &max_2);

    /**
       Optional function: called after each iterative approach if a manager class is
       run with verbosity level <= kPerIteration. Maybe useful for debugging.
    */
    virtual void Report();
    
    /// Function to reset the algorithm instance, called together with manager's Reset()
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
       Optional function: called at the beggining of each iterative loop.
       This provides all clusters' information in case the algorithm need them. Note this
       is called per iteration which may be more than once per event.
     */
    //virtual void IterationBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters);

    /**
       Optional function: called at the end of each iterative loop.
     */
    //virtual void IterationEnd();

	protected:
	
		float _theta_cut ;
		float _phi_cut	 ; 
		float _ratio_cut ;
  };
}
#endif
/** @} */ // end of doxygen group 


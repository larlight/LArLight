/**
 * \file CPAlgoQSum.hh
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CPAlgoQSum
 *
 * @author kazuhiro_NAME
 */

/** \addtogroup CMTool

    @{*/
#ifndef CPALGOQSUM_HH
#define CPALGOQSUM_HH

#include "CPriorityAlgoBase.hh"

namespace cmtool {
  /**
     \class CPAlgoQSum
     Simple algorithm to determine priority based on charge sum
     If charge sum < set cut value by a user, returns -1.     
  */
  class CPAlgoQSum : public CPriorityAlgoBase {
    
  public:
    
    /// Default constructor
    CPAlgoQSum();
    
    /// Default destructor
    virtual ~CPAlgoQSum(){};

    /**
       Core function: given the CPAN input, return a float which indicates 
       the user-defined priority for analysis.
    */
    virtual float Priority(const ::cluster::ClusterParamsAlgNew &cluster);

    /// Setter for minimum charge
    void SetMinQ(double v) { _qsum_cut = v; }

  protected:

    double _qsum_cut;

  };
}
#endif
/** @} */ // end of doxygen group 


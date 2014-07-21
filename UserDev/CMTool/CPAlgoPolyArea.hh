/**
 * \file CPAlgoPolyArea.hh
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CPAlgoPolyArea
 *
 * @author kazuhiro_NAME
 */

/** \addtogroup CMTool

    @{*/
#ifndef CPALGOPOLYAREA_HH
#define CPALGOPOLYAREA_HH

#include "CPriorityAlgoBase.hh"

namespace cmtool {
  /**
     \class CPAlgoPolyArea
     Simple algorithm to determine priority based on area of 2D polygon.
     If area < set cut value by a user, returns -1.
  */
  class CPAlgoPolyArea : public CPriorityAlgoBase {
    
  public:
    
    /// Default constructor
    CPAlgoPolyArea();
    
    /// Default destructor
    virtual ~CPAlgoPolyArea(){};

    /**
       Core function: given the CPAN input, return a float which indicates 
       the user-defined priority for analysis.
    */
    virtual float Priority(const ::cluster::ClusterParamsAlgNew &cluster);

    /// Setter for minimum area
    void SetMinArea(double area) { _area_cut = area; }

  protected:

    double _area_cut;
  };
}
#endif
/** @} */ // end of doxygen group 


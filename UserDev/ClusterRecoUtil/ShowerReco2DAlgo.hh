/**
 * \file ShowerReco2DAlgo.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class ShowerReco2DAlgo
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef SHOWERRECO2DALGO_HH
#define SHOWERRECO2DALGO_HH

#include <iostream>

namespace cluster {
  /**
     \class ShowerReco2DAlgo
     This class defines an algorithm that compiles all smaller algorithms that needs to complete
     the 2D shower cluster reconstruction. Such algorithms include: (1) shower cluster parameter
     reconstruction, (2) 2D cluster merging, (3) track/shower separation, and (3) 2D cluster
     matching algorithms.
     Note the interface will be kept independent of the framework (i.e. no LArSoft recob::Cluster
     or no LArLight larlight::cluster dependency). To run the algorithm in a particular framework,
     one needs to write a separate driver module which interface with specific data product definition.
  */
  class ShowerReco2DAlgo{
    
  public:
    
    /// Default constructor
    ShowerReco2DAlgo(){};
    
    /// Default destructor
    virtual ~ShowerReco2DAlgo(){};

  };
}

#endif
/** @} */ // end of doxygen group 


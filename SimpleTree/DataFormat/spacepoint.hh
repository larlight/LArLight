#ifndef SPACEPOINT_HH
#define SPACEPOINT_HH
/**
 * \file spacepoint.hh
 *
 * \ingroup SimpleTreeData
 * 
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeData

    @{*/

#include "data_base.hh"
#include "TVector3.h"
#include "TMatrixD.h"

/**
   \class sps
   LArSoft Spacepoints class equivalent data container. It holds C-array with 1 entry per spacepoint. 
*/
class sps : public data_base {

public:

  /// Default constructor
  sps();

  /// Default destructor
  virtual ~sps(){};

  /**
     Implementation data_base::clear_event. 
     When the argument boolean "all" is set to true, clear ALL C-array data elements to the default value.
     When "all" is set to false (default), only filled entries are cleared.
     This way we save time to loop over array elements.
  */
  virtual void clear_event(bool all=false);

  /// Implementation of track data address setter
  virtual void set_address(TTree* t);

  //--- Member getters ---//
  UShort_t        num_sps()     const { return _num_sps;     };
  const UShort_t* spsID()      const { return _spsID;      };
  const Float_t*  vtxx()       const { return _vtxx;       };
  const Float_t*  vtxy()       const { return _vtxy;       };
  const Float_t*  vtxz()       const { return _vtxz;       };
  const Float_t*  vtxx_sigma() const { return _vtxx_sigma; };
  const Float_t*  vtxy_sigma() const { return _vtxy_sigma; };
  const Float_t*  vtxz_sigma() const { return _vtxz_sigma; };
  const Float_t*  chi2()       const { return _chi2;       };

  /// Setter method to add a space point
  void add_spacepoint(UShort_t spsID, 
		      Float_t x,  Float_t y,  Float_t z,
		      Float_t ex, Float_t ey, Float_t ez,
		      Float_t chi2);

protected:

  UShort_t _num_sps;                            ///< Number of space-points (SPS)
  UShort_t _spsID[DATA::kMaxSpacePoints];      ///< SPS ID
  Float_t  _vtxx[DATA::kMaxSpacePoints];       ///< SPS x coordinates
  Float_t  _vtxy[DATA::kMaxSpacePoints];       ///< SPS y coordinates
  Float_t  _vtxz[DATA::kMaxSpacePoints];       ///< SPS z coordinates
  Float_t  _vtxx_sigma[DATA::kMaxSpacePoints]; ///< SPS x coordinate errors
  Float_t  _vtxy_sigma[DATA::kMaxSpacePoints]; ///< SPS y coordinate errors
  Float_t  _vtxz_sigma[DATA::kMaxSpacePoints]; ///< SPS z coordinate errors
  Float_t  _chi2[DATA::kMaxSpacePoints];       ///< SPS Chi2

};

#endif

/** @} */ // end of doxygen group 

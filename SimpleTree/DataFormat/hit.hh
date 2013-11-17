#ifndef HIT_HH
#define HIT_HH
/**
 * \file hit.hh
 *
 * \ingroup SimpleTreeData
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeData

    @{*/

#include "data_base.hh"
/**
   \class hit
   LArSoft Hit class equivalent data container that holds C-array with 1 entry per hit. Note it also holds an associated track ID as well.
*/
class hit : public data_base {

public:

  /// Default constructor
  hit(DATA::DATA_TYPE type=DATA::Hit);

  /// Default destructor
  virtual ~hit(){};

  /**
     Implementation data_base::clear_event. 
     When the argument boolean "all" is set to true, clear ALL C-array data elements to the default value.
     When "all" is set to false (default), only filled entries are cleared.
     This way we save time to loop over array elements.
  */
  virtual void clear_event(bool all=false);

  /// Implementation of track data address setter
  virtual void set_address(TTree* t);

  /// Setter method to add a hit
  void add_hit(UChar_t  plane, UShort_t wire,   UShort_t channel,
	       Double_t peakT, Double_t charge, Double_t ph,
	       UShort_t trackID);

  //--- Getter methods ---//
  UShort_t        no_hits() const { return _no_hits; };
  const UChar_t*  plane()   const { return _plane;   };
  const UShort_t* wire()    const { return _wire;    };
  const UShort_t* channel() const { return _channel; };
  const Double_t* peakT()   const { return _peakT;   };
  const Double_t* charge()  const { return _charge;  };
  const Double_t* ph()      const { return _ph;      };
  const UShort_t* trackID() const { return _trackID; };

protected:
  
  UShort_t _no_hits;                 ///< Number of hits
  UChar_t  _plane[DATA::kMaxHits];   ///< Wire plane ID
  UShort_t _wire[DATA::kMaxHits];    ///< Wire ID number
  UShort_t _channel[DATA::kMaxHits]; ///< Channel number
  Double_t _peakT[DATA::kMaxHits];   ///< Hit peak time
  Double_t _charge[DATA::kMaxHits];  ///< Hit charge
  Double_t _ph[DATA::kMaxHits];      ///< ???
  UShort_t _trackID[DATA::kMaxHits]; ///< Associated track ID

  /**
     - What is _hit_ph?
     - Associated cluster id?
     - Hit time width (sigma)?
  */

};

#endif

/** @} */ // end of doxygen group 

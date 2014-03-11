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
  virtual Bool_t set_address(TTree* t,Bool_t createa=false);

  /// Setter method to add a hit
  void add_hit(UChar_t  plane,  UShort_t wire,   UShort_t channel,
	       Double_t startT, Double_t peakT,  Double_t endT,
	       Double_t startT_sigma, Double_t peakT_sigma,  Double_t endT_sigma,
	       Double_t charge, Double_t charge_sigma,
	       UShort_t trackID);

  //--- Getter methods ---//
  UShort_t        num_hits() const { return _num_hits; };
  const UChar_t*  plane()   const { return _plane;   };
  const UShort_t* wire()    const { return _wire;    };
  const UShort_t* channel() const { return _channel; };

  const Double_t* startT()       const { return _startT;       };
  const Double_t* peakT()        const { return _peakT;        };
  const Double_t* endT()         const { return _endT;         };
  const Double_t* charge()       const { return _charge;       };
  const Double_t* startT_sigma() const { return _startT_sigma; };
  const Double_t* peakT_sigma()  const { return _peakT_sigma;  };
  const Double_t* endT_sigma()   const { return _endT_sigma;   };
  const Double_t* charge_sigma() const { return _charge_sigma; };
  const Double_t* gof()          const { return _gof;          };
  const UShort_t* trackID()      const { return _trackID;      };

protected:
  
  UShort_t _num_hits;                      ///< Number of hits
  UChar_t  _plane[DATA::kMaxHits];         ///< Wire plane ID
  UShort_t _channel[DATA::kMaxHits];       ///< Channel number
  UShort_t _wire[DATA::kMaxHits];          ///< Wire ID number
  Double_t _startT[DATA::kMaxHits];        ///< Hit start time
  Double_t _peakT[DATA::kMaxHits];         ///< Hit peak time
  Double_t _endT[DATA::kMaxHits];          ///< Hit end time
  Double_t _charge[DATA::kMaxHits];        ///< Hit charge
  Double_t _startT_sigma[DATA::kMaxHits];  ///< Hit start time
  Double_t _peakT_sigma[DATA::kMaxHits];   ///< Hit peak time
  Double_t _endT_sigma[DATA::kMaxHits];    ///< Hit end time
  Double_t _charge_sigma[DATA::kMaxHits];  ///< Hit charge
  Double_t _gof[DATA::kMaxHits];           ///< Goodness of hit
  UShort_t _trackID[DATA::kMaxHits];       ///< Associated track ID

  /**
     cluster ID not saved?
  */

};

#endif

/** @} */ // end of doxygen group 

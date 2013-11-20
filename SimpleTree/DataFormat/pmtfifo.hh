#ifndef PMTFIFO_HH
#define PMTFIFO_HH
/**
 * \file pmtfifo.hh
 *
 * \ingroup SimpleTreeData
 * 
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeData

    @{*/

#include "data_base.hh"

/**
   \class sps
   LArSoft FIFOChannel class equivalent data container. 
*/
class pmtfifo : public data_base {

public:

  /// Default constructor
  pmtfifo();

  /// Default destructor
  virtual ~pmtfifo(){};

  /**
     Implementation data_base::clear_event. 
     When the argument boolean "all" is set to true, clear ALL C-array data elements to the default value.
     When "all" is set to false (default), only filled entries are cleared.
     This way we save time to loop over array elements.
  */
  virtual void clear_event(bool all=false);

  /// Implementation of track data address setter
  virtual Bool_t set_address(TTree* t,Bool_t create=false);

  //--- Member getters ---//
  UShort_t        num_pmtfifo() const { return _num_pmtfifo; };
  const UShort_t* channel()     const { return _channel;     };
  const UChar_t*  discID()      const { return _discID;      };
  const UInt_t*   frame()       const { return _frame;       };
  const UInt_t*   sample()      const { return _sample;      };
  const UInt_t*   index()       const { return _index;       };
  const UShort_t* size()        const { return _size;        };
  UInt_t          num_adc()     const { return _num_adc;     };
  const UShort_t* adc()         const { return _adc;         };

  /// Setter method to add a PMT FIFO
  void add_pmtfifo(UChar_t channel, UChar_t disc,
		   UInt_t  frame,   UInt_t  sample,
		   const std::vector<UShort_t> wf);

protected:

  UShort_t _num_pmtfifo;                ///< Number of space-points (SPS)
  UShort_t _channel[DATA::kMaxPMTFIFO]; ///< Channel number
  UChar_t  _discID[DATA::kMaxPMTFIFO];  ///< Discriminator ID
  UInt_t   _frame[DATA::kMaxPMTFIFO];   ///< Frame ID
  UInt_t   _sample[DATA::kMaxPMTFIFO];  ///< Sample ID
  UInt_t   _index[DATA::kMaxPMTFIFO];   ///< Start index of corresponding ADC sample C-array
  UShort_t _size[DATA::kMaxPMTFIFO];    ///< Length of ADC sample C-array
  UInt_t   _num_adc;                    ///< Number of ADC count
  UShort_t _adc[DATA::kMaxPMTWords];    ///< ADC sample combined for all channel

};

#endif

/** @} */ // end of doxygen group 

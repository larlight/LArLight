/**
 * \file data_base.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Base class def for data class
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/
#ifndef DATA_BASE_HH
#define DATA_BASE_HH

#include "Base-TypeDef.hh"
#include <vector>
#include <limits>
#include <TObject.h>

namespace larlight{
  /**
     \class data_base
     A base coass for PMT-wise & event-wise data holder class
  */
  class data_base : public TObject {
    
  public:
    
    /// Default constructor
    data_base(DATA::DATA_TYPE type=DATA::DATA_TYPE_MAX) : TObject(){ clear_data(); _type=type; }
    
    /// Default copy constructor to avoid memory leak in ROOT streamer
    data_base(const data_base &original) : TObject(original),
					   fRunNumber(original.fRunNumber),
					   fSubRunNumber(original.fSubRunNumber),
					   fEventID(original.fEventID),
					   _type(original._type)
    {}

    
    /// Default destructor
    virtual ~data_base(){}
    
    /// Clear method
    virtual void clear_data(){fRunNumber = fSubRunNumber = fEventID = DATA::INVALID_UINT;}
    
    /// run number setter
    void set_run      (UInt_t run) { fRunNumber    = run; }
    /// sub-run number setter
    void set_subrun   (UInt_t run) { fSubRunNumber = run; }
    /// event-id setter
    void set_event_id (UInt_t id ) { fEventID      = id;  }
    
    /// run number getter
    UInt_t run      () const { return fRunNumber;    }
    /// sub-run number getter
    UInt_t subrun   () const { return fSubRunNumber; }
    /// event-id getter
    UInt_t event_id () const { return fEventID;      }
    /// data type getter
    DATA::DATA_TYPE data_type() const {return _type; }

  protected:
    
    /// Run number
    UInt_t fRunNumber;
    
    /// Sub-Run number
    UInt_t fSubRunNumber;
    
    /// Event ID
    UInt_t fEventID;

    /// DATA_TYPE
    DATA::DATA_TYPE _type;
    
    ////////////////////////
    ClassDef(data_base,2)
    ////////////////////////
      
  };
}
#endif
/** @} */ // end of doxygen group 

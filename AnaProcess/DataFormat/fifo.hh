/**
 * \file fifo.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for base FEM data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef FIFO_HH
#define FIFO_HH

#include "data_base.hh"

namespace larlight {

  /**
     \class fifo 
     Channel-wise data member class to hold a collection of ADC samples.
  */
  class fifo : public std::vector<UShort_t>, 
	       public data_base {
    
  public:

    /// Default constructor
    fifo(DATA::DATA_TYPE type=DATA::DATA_TYPE_MAX);
    
    /// Default copy constructor
    fifo(const fifo& original)
      : std::vector<UShort_t>(original),
	data_base(original),
	_channel_number(original._channel_number),
	_channel_frame_id(original._channel_frame_id),
	_timeslice(original._timeslice),
	_disc_id(original._disc_id)
    {}
    
    /// Setter for the channel number
    void set_channel_number   (UShort_t ch)   {_channel_number=ch;}

    /// Setter for the channel frame ID number
    void set_channel_frame_id (UInt_t id)        {_channel_frame_id=id;};
    
    /// Setter for the channel discriminator ID number
    void set_disc_id          (FEM::DISCRIMINATOR id) {_disc_id=id;};
    
    /// Setter for the timeslice number
    void set_timeslice        (UInt_t t)         {_timeslice=t;};
    
    /// Getter for the channel frame ID number
    UInt_t channel_frame_id()    const {return _channel_frame_id;};
    
    /// Getter for the discriminator ID number
    FEM::DISCRIMINATOR disc_id()      const {return _disc_id;};
    
    /// Getter for the timeslice number
    UInt_t timeslice()           const {return _timeslice;};
    
    /// Getter for the channel number
    UShort_t channel_number() const {return _channel_number;}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data();
    
    /// Default destructor
    ~fifo(){}
    
    
  protected:
    
    /// Internal function to reset variables.
    virtual void init_vars();

    UShort_t _channel_number;    ///< Channel number
    UInt_t   _channel_frame_id;  ///< Channel frame ID
    UInt_t   _timeslice;         ///< Timeslice number
    FEM::DISCRIMINATOR _disc_id; ///< Discriminator ID 
    
    ////////////////////////
    ClassDef(fifo,3)
    ////////////////////////
  };
  
  
  /**
     \class event_fifo 
     Event-wise data member class to hold a collection of ch-wise data members
  */
  class event_fifo : public std::vector<larlight::fifo>, public data_base {
    
  public:
    
    /// Default constructor ... provide an option to set the length of ch-wise data
    event_fifo(DATA::DATA_TYPE type=DATA::DATA_TYPE_MAX) : std::vector<larlight::fifo>(), data_base(type)
    {}
    
    /// Default copy constructor needed to avoid memory leak in ROOT streamer
    event_fifo(const event_fifo& original)
      : std::vector<larlight::fifo>(original),
	data_base(original),
	_event_frame_id(original._event_frame_id),
	_module_address(original._module_address),
	_module_id(original._module_id),
	_checksum(original._checksum),
	_nwords(original._nwords)
    {}
    
    /// Default destructor
    ~event_fifo(){}
    
    /// Setter for the event frame number
    void set_event_frame_id(UInt_t id) {_event_frame_id=id;}
    
    /// Setter for the module address from which this event is read-out
    void set_module_address(UInt_t id) {_module_address=id;}
    
    /// Setter for the module ID
    void set_module_id(UInt_t id)      {_module_id=id;}
    
    /// Setter for the checksum
    void set_checksum(UInt_t sum)      {_checksum=sum;}
    
    /// Setter for the # words read out
    void set_nwords(UInt_t n)          {_nwords=n;}
    
    /// Setter for the trigger frame id
    void set_trigger_frame_id(UInt_t id) {_trigger_frame_id=id;}
    
    /// Setter for the trigger timeslice
    void set_trigger_timeslice(UInt_t n) {_trigger_timeslice=n;}
    
    /// Getter for the frame ID
    UInt_t event_frame_id() const {return _event_frame_id;}
    
    /// Getter for the module address
    UInt_t module_address() const {return _module_address;}
    
    /// Getter for the module ID
    UInt_t module_id()      const {return _module_id;}
    
    /// Getter for the checksum
    UInt_t checksum()       const {return _checksum;} 
    
    /// Getter for the number of read words
    UInt_t nwords()         const {return _nwords;}
    
    /// Getter for the trigger frame id
    UInt_t trigger_frame_id() const {return _trigger_frame_id;}
    
    /// Getter for the trigger time slice
    UInt_t trigger_timeslice() const {return _trigger_timeslice;}
    
    /// A function to reset data member variables
    virtual void clear_data();
    
    
  protected:
    
    virtual void init_vars();
    /// Actual implementation function of resetting variables
    
    UInt_t _event_frame_id;       ///< event frame ID number
    UInt_t _module_address;       ///< module address number
    UInt_t _module_id;            ///< module ID number
    
    UInt_t _trigger_frame_id;     ///< trigger frame id
    UInt_t _trigger_timeslice;    ///< trigger time slice
    
    UInt_t _checksum;             ///< checksum of readout events
    UInt_t _nwords;               ///< # of event words readout
    
    ///////////////////////////
    ClassDef(event_fifo,3)
    //////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 


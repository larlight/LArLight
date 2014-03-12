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
    fifo(DATA::DATA_TYPE type=DATA::FIFO);
    
    /// Default copy constructor
    fifo(const fifo& original)
      : std::vector<UShort_t>(original),
	data_base(original),
	_channel_number(original._channel_number),
	_module_address(original._module_address),
	_module_id(original._module_id),
	_readout_frame_number(original._readout_frame_number),
	_readout_sample_number(original._readout_sample_number)
    {}

    /// Fast vector copy constructor
    fifo(UShort_t ch,
	 UInt_t   frame,
	 UInt_t   sample,
	 UChar_t  module_address,
	 UChar_t  module_id,
	 DATA::DATA_TYPE type,
	 std::vector<UShort_t> wf) : std::vector<UShort_t>(wf),
				     data_base(type),
				     _channel_number(ch),
				     _module_address(module_address),
				     _module_id(module_id),
				     _readout_frame_number(frame),
				     _readout_sample_number(sample)
    {}
    

    /// Setter for the channel number
    void set_channel_number (UShort_t ch)     {_channel_number=ch;}

    /// Setter for the channel frame ID number
    void set_readout_frame_number (UInt_t id) {_readout_frame_number=id;}
    
    /// Setter for the readout_sample_number number
    void set_readout_sample_number (UInt_t t) {_readout_sample_number=t;}

    /// Setter for the module address
    void set_module_address(UChar_t n) { _module_address = n;}

    /// Setter for the module id
    void set_module_id(UChar_t n) { _module_id = n;}
    
    /// Getter for the channel frame ID number
    UInt_t readout_frame_number() const {return _readout_frame_number;}
    
    /// Getter for a RAW readout sample number
    UInt_t readout_sample_number_RAW() const { return _readout_sample_number; }
    
    /// Getter for the readout_sample_number number
    inline UInt_t readout_sample_number_2MHz() const  
    {return (_type==DATA::PMTFIFO ? _readout_sample_number/32 : _readout_sample_number   ); }

    /// Getter for the readout_sample_number number
    inline UInt_t readout_sample_number_16MHz() const  
    {return (_type==DATA::PMTFIFO ? _readout_sample_number/4  : _readout_sample_number*8 ); }

    /// Getter for the readout_sample_number number
    inline UInt_t readout_sample_number_64MHz() const  
    {return (_type==DATA::PMTFIFO ? _readout_sample_number    : _readout_sample_number*32 ); }
    
    /// Getter for the channel number
    UShort_t channel_number() const {return _channel_number;}

    /// Getter for the module address
    UChar_t module_address() const { return _module_address; }
    
    /// Getter for the module id
    UChar_t module_id() const {return _module_id;}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data();
    
    /// Default destructor
    ~fifo(){}
    
    
  protected:
    
    /// Internal function to reset variables.
    virtual void init_vars();

    UShort_t _channel_number;            ///< Channel number
    UChar_t  _module_address;            ///< Module number
    UChar_t  _module_id;                 ///< Module ID
    UInt_t   _readout_frame_number;      ///< Frame number
    UInt_t   _readout_sample_number;     ///< Sample number
    
    ////////////////////////
    ClassDef(fifo,5)
    ////////////////////////
  };
  
  
  /**
     \class event_fifo 
     Event-wise data member class to hold a collection of ch-wise data members
  */
  class event_fifo : public std::vector<larlight::fifo>, public event_base {
    
  public:
    
    /// Default constructor ... provide an option to set the length of ch-wise data
    event_fifo(DATA::DATA_TYPE type=DATA::FIFO);
    
    /// Default copy constructor needed to avoid memory leak in ROOT streamer
    event_fifo(const event_fifo& original)
      : std::vector<larlight::fifo>(original),
	event_base(original),
	_event_number(original._event_number),
	_event_frame_number(original._event_frame_number),
	_module_address(original._module_address),
	_module_id(original._module_id),
	_fem_trig_frame_number(original._fem_trig_frame_number),
	_fem_trig_sample_number(original._fem_trig_sample_number),
	_checksum(original._checksum),
	_nwords(original._nwords)
    {}
    
    /// Default destructor
    ~event_fifo(){}

    /// Setter for the evnet number
    void set_event_number(UInt_t n) { _event_number = n; }
    
    /// Setter for the event frame number
    void set_event_frame_number(UInt_t n) {_event_frame_number=n;}
    
    /// Setter for the module address from which this event is read-out
    void set_module_address(UChar_t n) {_module_address=n;}
    
    /// Setter for the module ID
    void set_module_id(UChar_t id)      {_module_id=id;}
    
    /// Setter for the checksum
    void set_checksum(UInt_t sum)      {_checksum=sum;}
    
    /// Setter for the # words read out
    void set_nwords(UInt_t n)          {_nwords=n;}
    
    /// Setter for the trigger frame id
    void set_fem_trig_frame_number(UInt_t n) {_fem_trig_frame_number=n;}
    
    /// Setter for the trigger timeslice
    void set_fem_trig_sample_number(UInt_t n) {_fem_trig_sample_number=n;}

    /// Getter for the event number
    UInt_t event_number() const { return _event_number;}
    
    /// Getter for the frame ID
    UInt_t event_frame_number() const {return _event_frame_number;}
    
    /// Getter for the module address
    UChar_t module_address() const {return _module_address;}
    
    /// Getter for the module ID
    UChar_t module_id()      const {return _module_id;}
    
    /// Getter for the checksum
    UInt_t checksum()       const {return _checksum;} 
    
    /// Getter for the number of read words
    UInt_t nwords()         const {return _nwords;}
    
    /// Getter for the trigger frame number
    UInt_t fem_trig_frame_number() const {return _fem_trig_frame_number;}

    /// Getter for the fem trigger sample number RAW
    inline UInt_t fem_trig_sample_number_RAW() const {return _fem_trig_sample_number;}
    
    /// Getter for the fem trigger sample number in 2 MHz
    inline UInt_t fem_trig_sample_number_2MHz() const {return _fem_trig_sample_number;}    

    /// Getter for the fem trigger sample number in 16 MHz
    inline UInt_t fem_trig_sample_number_16MHz() const {return _fem_trig_sample_number*8;}

    /// Getter for the fem trigger sample number in 64 MHz
    inline UInt_t fem_trig_sample_number_64MHz() const {return _fem_trig_sample_number*32;}
    
    /// A function to reset data member variables
    virtual void clear_data();
    
    
  protected:
    
    virtual void init_vars();
    /// Actual implementation function of resetting variables
    UInt_t  _event_number;              ///< event number counter
    UInt_t  _event_frame_number;        ///< event frame ID number
    UChar_t _module_address;            ///< module address number
    UChar_t _module_id;                 ///< module ID number
    
    UInt_t _fem_trig_frame_number;     ///< trigger frame id
    UInt_t _fem_trig_sample_number;    ///< trigger time slice
    
    UInt_t _checksum;                  ///< checksum of readout events
    UInt_t _nwords;                    ///< # of event words readout
    
    ///////////////////////////
    ClassDef(event_fifo,6)
    //////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 


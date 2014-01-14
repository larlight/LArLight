/**
 * \file trigger.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for trigger
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/
#ifndef TRIGGER_HH
#define TRIGGER_HH

#include "data_base.hh"

namespace larlight {

  /**
     \class trigger
     A class to hold trigger module output data
  */
  class trigger : public data_base {
    
  public:
    
    /// Default constructor
    trigger() : data_base() { init_vars(); };
    
    /// Default copy constructor to avoid memory leak in ROOT streamer
    trigger(const trigger &original);
    
    /// Default destructor
    virtual ~trigger(){};
    
    /// Setter for 16 MHz reminder
    void set_reminder_16MHz(UShort_t remin){_reminder_16MHz=remin;};
    
    /// Setter for 64 MHz reminder
    void set_reminder_64MHz(UShort_t remin){_reminder_64MHz=remin;};
    
    /// Setter for frame id
    void set_trig_frame_id(UInt_t id){_trig_frame_id=id;};
    
    /// Setter for sample id
    void set_trig_timeslice(UInt_t t){_trig_timeslice=t;};
    
    /// Setter for trigger ID
    void set_trig_id(UShort_t id){_trig_id=id;};
    
    /// Setter for PMT trigger data
    void set_pmt_data(UShort_t data){_pmt_data=data;};
    
    /// Getter for 16 MHz reminder
    UShort_t reminder_16MHz() const {return _reminder_16MHz;};

    /// Getter for 64 MHz reminder
    UShort_t reminder_64MHz() const {return _reminder_64MHz;};
    
    /// Getter for frame id
    UInt_t trig_frame_id() const {return _trig_frame_id;};
    
    /// Getter for sample id
    UInt_t trig_timeslice() const {return _trig_timeslice;};
    
    /// Getter for trigger ID
    UInt_t trig_id() const {return _trig_id;};
    
    /// Getter for PMT trigger data
    UShort_t pmt_data() const {return _pmt_data;};
    
    /// Setter for trigger bits
    void set_trigger_bits(Bool_t trig_pc,  Bool_t trig_ext,
			  Bool_t active,   Bool_t gate1_in,
			  Bool_t gate2_in, Bool_t veto_in,
			  Bool_t calib)
    { _trig_pc  = trig_pc;   _trig_ext = trig_ext;
      _active   = active;    _gate1_in = gate1_in;
      _gate2_in = gate2_in; _veto_in  = veto_in;
      _calib    = calib;
    };
    
    /// Getter for trigger bit
    Bool_t trig_pc() const {return _trig_pc;};
    
    /// Getter for trigger bit
    Bool_t trig_ext() const {return _trig_ext;};
    
    /// Getter for trigger bit
    Bool_t active() const {return _active;};
    
    /// Getter for trigger bit
    Bool_t gate1() const {return _gate1_in;};
    
    /// Getter for trigger bit
    Bool_t gate2() const {return _gate2_in;};
    
    /// Getter for trigger bit
    Bool_t veto_in() const {return _veto_in;};
    
    /// Getter for trigger big
    Bool_t calib() const {return _calib;};
    
    /// Clear method override
    virtual void clear_data();
    
  protected:

    void init_vars();
    
    UShort_t  _trig_timeslice;
    UInt_t    _trig_frame_id;
    UInt_t    _trig_id;
    UShort_t  _pmt_data;
    Bool_t    _trig_pc;
    Bool_t    _trig_ext;
    Bool_t    _active;
    Bool_t    _gate1_in;
    Bool_t    _gate2_in;
    Bool_t    _veto_in;
    Bool_t    _calib;
    UShort_t  _reminder_64MHz;
    UShort_t  _reminder_16MHz;
    
    ////////////////////////
    ClassDef(trigger,3)
    ////////////////////////
    
  };
}

#endif
/** @} */ // end of doxygen group 
